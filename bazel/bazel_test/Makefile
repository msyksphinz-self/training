.PHONY: all
all: run

BAZEL=$(shell which bazel)

.PHONY: run
run:
	$(BAZEL) run //src:main

.PHONY: build
build :
	$(BAZEL) build //src:main

.PHONY: clean
clean :
	$(BAZEL) clean
