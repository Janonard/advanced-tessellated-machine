# Contributing to ATM

So you want to contribute to ATM and make it a bit better than it was before?
Read this to get to know to do it!

## Bug reports

Let's assume you have found a bug. What do you do? First, you search the issue
site of ATM for your bug, maybe someone already reported it. If this is the case,
you may comment that you've encountered this bug too.

If nobody found your bug, please create an issue with a bug report. It should contain:

* An appropriate title that briefly explains what your bug is about
* What you have done to encounter the bug
* What you expected to happen
* What happened instead
* If possible, add a test program that reproduces the bug.

After submitting your report, you have to wait until someone answers it. Maybe they
will ask you more questions about that bug, so please answer them. Then, someone will
start to fix it and when this is done, the issue will be closed.

## Submitting patches

You have fixed a bug or added a new feature and you think that it should be in upstream
ATM? Follow these steps:

* Create a fork of ATM on Github
* Do your changes in the fork
* Create a pull request for your changes. The description should contain at least these information:
  * What does your code do? Does it add a feature? Does it fix a bug? If you've fixed a bug, please also add the issue number of it.
  * Why should it be part of ATM? A bugfix it quite self-explaining, but features need a reason to be there.
* Wait for questions from a maintainer and answer them.
* Fix problems with your code the maintainer has found.
* Wait for the pull request to be merged.

As you might have guessed, there are some rules every patch has to obey to:

* Every bugfix/feature needs a test to check whether it is working or not. Please place a test program that `dCrash`es when it fails
and add it to the `CTestInfo.cmake` file.
* The code should be easy to understand, maintain and extend. Comment all header files appropriatly.
* Your code should adapt to the coding style of the whole source code.
