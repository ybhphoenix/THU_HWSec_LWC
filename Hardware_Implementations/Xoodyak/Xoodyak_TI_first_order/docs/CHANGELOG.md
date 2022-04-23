# Changelog

All notable changes to this implementation variant **Xoodyak_TI_first_order** will be documented in this file.

## [Unreleased]

- Reduce the cycle counts of Xoodoo permutation from 36 to 24.

## [0.1.1] - 2022-04-23

### Fixed

- Fix the handshake of random data input (`rdi`).

### Changed

- Change the port width of `rdi` from 768 bits to 384 bits.

### Removed

- Remove parameter `roundsPerCycle` in `xoodoo_n_rounds_SCA.v`, which has not been used and verified.

## [0.1.0] - 2022-04-18

### Added

- Protect hardware implementation for Xoodyak with 1st order masking using TI.