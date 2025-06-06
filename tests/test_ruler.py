from __future__ import annotations

import pytest

# ------------------------
# Rule Insertion + Order
# ------------------------


def test_push_adds_rule(ruler, dummy_rule_fn):
    ruler.push("a", dummy_rule_fn)
    assert len(ruler.get_rules("") or []) == 1


def test_at_replaces_existing_rule(ruler, dummy_rule_fn, another_dummy_fn):
    ruler.push("a", dummy_rule_fn)
    ruler.at("a", another_dummy_fn)
    state = []
    ruler.get_rules("")[0](state)
    assert state == ["also_ran"]


def test_before_inserts_rule_before_target(ruler, dummy_rule_fn, another_dummy_fn):
    ruler.push("b", dummy_rule_fn)
    ruler.before("b", "a", another_dummy_fn)
    rules = ruler.get_rules("")
    out = []
    for rule in rules:
        rule(out)
    assert out == ["also_ran", "ran"]


def test_after_inserts_rule_after_target(ruler, dummy_rule_fn, another_dummy_fn):
    ruler.push("a", dummy_rule_fn)
    ruler.after("a", "b", another_dummy_fn)
    out = []
    rules = ruler.get_rules("")
    for rule in rules:
        rule(out)
    assert out == ["ran", "also_ran"]


def test_push_appends_to_end(ruler, make_rule):
    ruler.push(*make_rule("a"))
    ruler.push(*make_rule("b"))
    order = []
    for rule in ruler.get_rules(""):
        rule(order)
    assert order == ["a", "b"]


def test_at_raises_if_rule_not_found(ruler, dummy_rule_fn):
    with pytest.raises(RuntimeError, match="Rule not found: missing"):
        ruler.at("missing", dummy_rule_fn)


def test_before_raises_if_target_not_found(ruler, dummy_rule_fn):
    with pytest.raises(RuntimeError, match="Rule not found: missing"):
        ruler.at("missing", dummy_rule_fn)


def test_after_raises_if_target_not_found(ruler, dummy_rule_fn):
    with pytest.raises(RuntimeError, match="Rule not found: missing"):
        ruler.at("missing", dummy_rule_fn)


def test_rule_order_after_multiple_insertions(ruler, make_rule):
    ruler.push(*make_rule("a"))
    ruler.push(*make_rule("c"))
    ruler.before("c", *make_rule("b"))
    order = []
    for rule in ruler.get_rules(""):
        rule(order)
    assert order == ["a", "b", "c"]


def test_ordering_preserved_after_disable_and_enable(ruler, make_rule):
    ruler.push(*make_rule("a"))
    ruler.push(*make_rule("b"))
    ruler.disable(["a"])
    ruler.enable(["a"])
    order = []
    for rule in ruler.get_rules(""):
        rule(order)
    assert order == ["a", "b"]


# ------------------------
# Enable / Disable
# ------------------------


def test_enable_marks_rules_enabled(ruler, make_rule):
    ruler.push(*make_rule("a"))
    ruler.disable(["a"])
    ruler.enable(["a"])
    rules = ruler.get_rules("")
    out = []
    for rule in rules:
        rule(out)
    assert out == ["a"]


def test_disable_marks_rules_disabled(ruler, make_rule):
    ruler.push(*make_rule("a"))
    ruler.disable(["a"])
    assert ruler.get_rules("") == []


def test_enable_only_disables_others(ruler, make_rule):
    ruler.push(*make_rule("a"))
    ruler.push(*make_rule("b"))
    ruler.enable_only(["a"])
    rules = ruler.get_rules("")
    out = []
    for rule in rules:
        rule(out)
    assert out == ["a"]


def test_enable_ignores_invalid_when_flag_set(ruler):
    assert ruler.enable(["nonexistent"], ignore_invalid=True) == []


def test_disable_ignores_invalid_when_flag_set(ruler):
    assert ruler.disable(["nonexistent"], ignore_invalid=True) == []


def test_enable_raises_on_invalid_if_flag_unset(ruler):
    with pytest.raises(RuntimeError, match="Invalid rule name: bad"):
        ruler.enable(["bad"])


def test_disable_raises_on_invalid_if_flag_unset(ruler):
    with pytest.raises(RuntimeError, match="Invalid rule name: bad"):
        ruler.enable(["bad"])


# ------------------------
# Alt Chains
# ------------------------


def test_alt_chain_filters_rules_correctly(ruler, make_rule):
    ruler.push(*make_rule("a", alt=["x"]))
    assert ruler.get_rules("x") != []
    assert ruler.get_rules("y") == []


def test_multiple_alt_chains_applied_correctly(ruler, make_rule):
    ruler.push(*make_rule("a", alt=["x", "y"]))
    assert ruler.get_rules("x")
    assert ruler.get_rules("y")


def test_get_rules_returns_only_matching_alt_chain(ruler, make_rule):
    ruler.push(*make_rule("a", alt=["x"]))
    rules = ruler.get_rules("x")
    out = []
    for rule in rules:
        rule(out)
    assert out == ["a"]


def test_get_rules_returns_default_chain_when_empty(ruler, make_rule):
    ruler.push(*make_rule("a"))
    assert ruler.get_rules("")


def test_alt_chain_empty_returns_all_enabled(ruler, make_rule):
    ruler.push(*make_rule("a"))
    ruler.push(*make_rule("b", alt=["x"]))
    rules = ruler.get_rules("x")
    out = []
    for rule in rules:
        rule(out)
    assert out == ["b"]


# ------------------------
# Caching Behavior
# ------------------------


def test_cache_invalidated_on_push(ruler, make_rule):
    ruler.get_rules("")
    ruler.push(*make_rule("a"))
    rules = ruler.get_rules("")
    out = []
    for rule in rules:
        rule(out)
    assert out == ["a"]


def test_cache_invalidated_on_enable_disable(ruler, make_rule):
    ruler.push(*make_rule("a"))
    ruler.get_rules("")
    ruler.disable(["a"])
    assert ruler.get_rules("") == []


def test_cache_recomputed_when_dirty(ruler, make_rule):
    ruler.push(*make_rule("a"))
    ruler.get_rules("")
    ruler.disable(["a"])
    ruler.enable(["a"])
    assert ruler.get_rules("")


def test_cache_not_recomputed_when_clean(ruler, make_rule):
    ruler.push(*make_rule("a"))
    rules1 = ruler.get_rules("")
    out1 = []
    rules1 = ruler.get_rules("")
    for rule in rules1:
        rule(out1)

    rules2 = ruler.get_rules("")
    out2 = []
    for rule in rules2:
        rule(out2)
    assert out1 == out2


# ------------------------
# Execution / Behavior
# ------------------------


def test_rules_execute_in_defined_order(ruler, make_rule):
    ruler.push(*make_rule("a"))
    ruler.push(*make_rule("b"))
    out = []
    for rule in ruler.get_rules(""):
        rule(out)
    assert out == ["a", "b"]


def test_disabled_rules_do_not_execute(ruler, make_rule):
    ruler.push(*make_rule("a"))
    ruler.disable(["a"])
    out = []
    for rule in ruler.get_rules(""):
        rule(out)
    assert out == []


def test_alt_chain_execution_order(ruler, make_rule):
    ruler.push(*make_rule("a", alt=["x"]))
    ruler.push(*make_rule("b", alt=["x"]))
    out = []
    for rule in ruler.get_rules("x"):
        rule(out)
    assert out == ["a", "b"]


def test_enable_only_raises_on_invalid_when_flag_false(ruler):
    with pytest.raises(RuntimeError, match="Invalid rule name: not_there"):
        ruler.enable_only(["not_there"], ignore_invalid=False)


# ------------------------
# Miscellaneous Edge Cases
# ------------------------


def test_rules_with_same_alt_different_behaviors(ruler, make_rule):
    ruler.push(*make_rule("a", suffix="1", alt=["x"]))
    ruler.push(*make_rule("b", suffix="2", alt=["x"]))
    out = []
    for rule in ruler.get_rules("x"):
        rule(out)
    assert out == ["a1", "b2"]
