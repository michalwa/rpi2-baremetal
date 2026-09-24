- https://wiki.osdev.org/Raspberry_Pi_Bare_Bones
- Download arm-none-eabi toolchain for Windows from https://gitlab.arm.com/api/v4/projects/tooling%2Fgnu-toolchains-for-arm/packages/generic/gnu-toolchain/15.3.rel1/arm-gnu-toolchain-15.3.rel1-mingw-w64-x86_64-arm-none-eabi.zip into `toolchain/`

## To do

- [ ] Figure out how to pass control on all 4 cores to C (per-core stacks)
- [ ] Find or port a GPIO driver for the BCM2836R
- [ ] Investigate `wfe` and suspend/wake mechanisms in ARMv7
- [ ] Set up proper clock reads for timers
