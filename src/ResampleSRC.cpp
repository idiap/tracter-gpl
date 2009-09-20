/*
 * Copyright 2008 by IDIAP Research Institute
 *                   http://www.idiap.ch
 *
 * See the file COPYING for the licence associated with this software.
 */

#include <vector>
#include <cassert>

#include <samplerate.h>

#include "Resample.h"

/**
 * The class specific data for ResampleSRC
 */
struct Tracter::ResampleData
{
    SRC_STATE* state;
    SRC_DATA data;
    std::vector<float> resample;
};

/**
 * Initialise a sample rate converter.  Environment variables:
 *  - TargetFreq (16000)  Target sample frequency.
 */
Tracter::Resample::Resample(
    Component<float>* iInput, const char* iObjectName
)
{
    mObjectName = iObjectName;
    mInput = iInput;

    double targetRate = GetEnv("TargetRate", 16000);
    mResampleData = new ResampleData;
    ResampleData& r = *mResampleData;

    r.data.src_ratio = targetRate / mInput->FrameRate();
    assert(src_is_valid_ratio(r.data.src_ratio));

    int error;
    r.state = src_new(SRC_SINC_BEST_QUALITY, 1, &error);
    if (!r.state)
        throw Exception("%s: SRC error %s\n",
                        mObjectName, src_strerror(error));

    Connect(mInput, 1.0f / r.data.src_ratio);
    Verbose(1, "SRC Initialised\n");
}

/**
 * Deletes the sample rate converter
 */
Tracter::Resample::~Resample() throw()
{
    ResampleData& r = *mResampleData;
    src_delete(r.state);
    delete mResampleData;
    mResampleData = 0;
}

/**
 * Ensures that the input plugin has the right size given the rate conversion
 */
void Tracter::Resample::MinSize(int iSize, int iReadBack, int iReadAhead)
{
    // First call the base class to resize this cache
    assert(iSize > 0);
    ComponentBase::MinSize(iSize, iReadBack, iReadAhead);

    // Set the input buffer big enough to service largest output requests
    assert(mInput);
    ResampleData& r = *mResampleData;
    int minSize = (int)((double)iSize / r.data.src_ratio + 0.5);
    ComponentBase::MinSize(mInput, minSize, 0, 0);

    /* It's too complcated without an intermediate array */
    r.resample.resize(iSize);
}

void Tracter::Resample::Reset(bool iPropagate)
{
    ResampleData& r = *mResampleData;
    src_reset(r.state);
    CachedComponent<float>::Reset(iPropagate);
}

int Tracter::Resample::Fetch(IndexType iIndex, CacheArea& iOutputArea)
{
    assert(iOutputArea.Length() > 0);
    assert(iIndex >= 0);
    CacheArea inputArea;

    ResampleData& r = *mResampleData;
    IndexType index = (IndexType)((double)iIndex / r.data.src_ratio);
    int nGet = (int)((double)iOutputArea.Length() / r.data.src_ratio);
    int nGot = mInput->Read(inputArea, index, nGet);
    int nOut = (int)((double)nGot * r.data.src_ratio);
    Verbose(2, "i=%ld Get=%d Got=%d Out=%d len0=%d len1=%d\n",
            index, nGet, nGot, nOut, inputArea.len[0], inputArea.len[1]);

    /* Run it over the first circular block */
    r.data.data_in = mInput->GetPointer(inputArea.offset);
    r.data.data_out = &r.resample[0];
    r.data.input_frames = inputArea.len[0];
    r.data.output_frames = r.resample.size();
    r.data.end_of_input = ((nGot < nGet) && (inputArea.len[1] == 0));
    int error = src_process(r.state, &r.data);
    if (error)
        throw Exception("%s: src_process error %s\n",
                        mObjectName, src_strerror(error));

    /* Run again if there's a second circular block */
    if (inputArea.len[1] > 0)
    {
        r.data.data_in = mInput->GetPointer();
        r.data.data_out = &r.resample[r.data.output_frames_gen];
        r.data.input_frames = inputArea.len[1];
        r.data.output_frames = r.resample.size()-r.data.output_frames_gen;
        r.data.end_of_input = (nGot < nGet);
        error = src_process(r.state, &r.data);
        if (error)
            throw Exception("%s: src_process error %s\n",
                            mObjectName, src_strerror(error));
    }

    /* Copy the resampled data to the output */
    float* output = GetPointer(iOutputArea.offset);
    for (int i=0; i<iOutputArea.len[0]; i++)
        output[i] = r.resample[i];
    output = GetPointer();
    for (int i=0; i<iOutputArea.len[1]; i++)
        output[i] = r.resample[iOutputArea.len[0] + i];

    return nOut;
}
