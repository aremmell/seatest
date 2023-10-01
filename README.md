# seatest

seatest&mdash;a featherweight cross-platform C17 test framework.

<!-- SPDX-License-Identifier: MIT -->
<!-- Copyright (c) 2023 Ryan M. Lederman <lederman@gmail.com> -->

[![Quality Gate Status](https://sonarcloud.io/api/project_badges/measure?project=aremmell_seatest&metric=alert_status)](https://sonarcloud.io/summary/new_code?id=aremmell_seatest)
[![Maintainability Rating](https://sonarcloud.io/api/project_badges/measure?project=aremmell_seatest&metric=sqale_rating)](https://sonarcloud.io/summary/new_code?id=aremmell_seatest)
[![Reliability Rating](https://sonarcloud.io/api/project_badges/measure?project=aremmell_seatest&metric=reliability_rating)](https://sonarcloud.io/summary/new_code?id=aremmell_seatest)
[![Security Rating](https://sonarcloud.io/api/project_badges/measure?project=aremmell_seatest&metric=security_rating)](https://sonarcloud.io/summary/new_code?id=aremmell_seatest)

## WIP
seatest is a work-in-progress, as it is essentially brand new. Luckily I have most of what I need on-hand already; I should have screenshots and code samples within the next few days, and something worth trying out within a week.

## What
A minimalist, no-nonsense portable C17 framework for quickly creating your own custom test suites based on boilerplate code. Full support for conditional evaluation, warnings, fatal errors, command-line control, and even special requirements for tests (*if requirements are not met, the test is skipped and a warning is emitted*).

## Why
I designed a simplistic version of what is now `seatest` for [libsir](https://github.com/aremmell/libsir), because I didn't want *any* dependencies, even if it was just a test framework. Later on, I found myself porting that test rig multiple times, and on the third time, I decided the right thing to do is to create something reusable that others can utilize as well.
