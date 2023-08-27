#!/bin/bash -e

STAGE=$1
BACKEND=$2

source $(dirname $0)/init.sh

export HOME=`pwd`
git clone https://github.com/spack/spack ../spack || true
(
   cd ../spack
   git pull
)
source ../spack/share/spack/setup-env.sh

VARIANTS=""
if [ "$BACKEND" = "FFTW" ]; then
   VARIANTS="+fftw"
elif [ "$BACKEND" = "MKL" ]; then
   VARIANTS="+mkl"
elif [ "$BACKEND" = "CUDA" ]; then
   VARIANTS="+cuda cuda_arch=70 ^cuda@11.8.0"
elif [ "$BACKEND" = "ROCM" ]; then
   VARIANTS="+rocm amdgpu_target=gfx90a ^hip@5.5.0"
fi

SPEC="heffte@develop $VARIANTS ^$MPI %$COMPILER"
echo SPEC=$SPEC

if [ "$STAGE" = "build" ]; then
   rm -rf .spack
   spack compiler find
   spack install --only=dependencies --fresh $SPEC
   spack uninstall -a -y heffte || true
   spack dev-build -i --fresh $SPEC
elif [ "$STAGE" = "test" ]; then
   spack uninstall -a -y heffte || true
   spack dev-build -i --fresh --test=root $SPEC
else # STAGE = smoketest
   # Remove MPI load once spack package test() is corrected.
   spack load --first $MPI %$COMPILER
   spack test run heffte
fi
