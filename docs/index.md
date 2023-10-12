Welcome to the documentation for seatest, a featherweight cross-platform C17 test framework. Designed with *simplicity*, *completeness*, and *practicality* in mind, perhaps it is the right test framework for your project.

## Highlights

* Won't even make a dent in the size of your deployables–the static library weighs in at xxKiB, and the shared library at XXKiB
* A built-in command-line interface which makes both manual and automated (*i.e. CI*) invocation more flexible and customizable
* A comprehensive collection of evaluator and helper macros are available in order to facilitate writing expressive, complete, and straightforward tests
* Provides for special "conditions" that may optionally be used to prevent certain tests from running at all when the condition(s) are not present. For example, if a test absolutely requires an Internet connection in order to pass, that test can be skipped instead of failing.
* When a test fails, you will know precisely why, as well as where.

## How it works

The public interface to seatest is primarily macro-driven. This is by design, and allows repetitive, easily-forgettable code to be replaced by one-liners. This applies not only to the setup and configuration of the framework itself, but in the implementation of the tests.

If you are anxious to just see an example test rig, there's one [right here](https://github.com/aremmell/seatest/blob/master/example/example.c). If you would rather take your time and learn about the features and functionality available using the seatest framework, check out the [documentation](documentation.md).

