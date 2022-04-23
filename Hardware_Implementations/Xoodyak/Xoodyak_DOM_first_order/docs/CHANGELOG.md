# Changelog

All notable changes to this implementation variant **Xoodyak_DOM_first_order** will be documented in this file.

## [Unreleased]

- Reduce the cycle counts of Xoodoo permutation from 24 to 12.

## [0.1.1] - 2022-04-23

### Fixed

- Fix the handshake of random data input (`rdi`).

### Removed

- Remove parameter `roundsPerCycle` in `xoodoo_n_rounds_SCA.v`, which has not been used and verified.

## [0.1.0] - 2022-04-18

### Added

- Protect hardware implementation for Xoodyak with 1st order masking using DOM.