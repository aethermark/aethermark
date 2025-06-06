from __future__ import annotations

import pytest

from aethermark import RuleOptions, Ruler

# ------------------------
# Ruler Fixtures
# ------------------------


@pytest.fixture
def ruler():
    return Ruler()


@pytest.fixture
def dummy_rule_fn():
    def rule(state):
        state.append("ran")

    return rule


@pytest.fixture
def another_dummy_fn():
    def fn(state):
        state.append("also_ran")

    return fn


@pytest.fixture
def make_rule():
    def _make_rule(name, suffix="", alt=None):
        def fn(state):
            state.append(name + suffix)

        fn.__name__ = "fn"
        opts = RuleOptions()
        if alt:
            opts.alt = alt
        return name, fn, opts

    return _make_rule


# ------------------------
# Execution State Fixture (for rule functions)
# ------------------------


@pytest.fixture
def state():
    return []
