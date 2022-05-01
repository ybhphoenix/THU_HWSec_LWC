# Changelog

All notable changes to this implementation variant **Xoodyak_DOM_first_order** will be documented in this file.

## [0.2.1] - 2022-04-30

### Changed

- Reduce the cycle counts of Xoodoo permutation from 24 to 12.

## [0.2.0] - 2022-04-24

### Security

- Registers in `xoodoo_round_SCA.v` were put in wrong position in versions 0.1.x, leading to a side-channel leakage, which is fixed in this version.  

## [0.1.1] - 2022-04-23

### Fixed

- Fix the handshake of random data input (`rdi`).

### Removed

- Remove parameter `roundsPerCycle` in `xoodoo_n_rounds_SCA.v`, which has not been used and verified.

## [0.1.0] - 2022-04-18

### Added

- Protect hardware implementation for Xoodyak with 1st order masking using DOM.