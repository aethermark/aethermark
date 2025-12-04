## Changelog for Version 0.0.19

### ♻️ Refactoring
- 873bd31 refactor: rename library from Quantamark to Aethermark and update related files (MukulWaval, 2025-10-21 15:14)
- 167e9bc refactor: streamline workflow scripts and update Makefile targets for clarity (MukulWaval, 2025-10-21 14:59)
- cdd83cd refactor: enhance changelog workflow to safely determine target branch and rebase before pushing updates (MukulWaval, 2025-10-21 14:18)
- 4cb84bf refactor: optimize CI workflow by caching APT packages and build artifacts, and streamline Python environment setup (MukulWaval, 2025-10-21 13:17)
- 613a4d5 refactor: update CI workflow to install GoogleTest and improve Python test execution (MukulWaval, 2025-10-21 13:10)

### 📝 Documentation Updates
- d188649 docs: update CONTRIBUTING.md and Makefile for C++ module installation and usage (MukulWaval, 2025-10-21 17:01)
- f26d8cf docs: fix badges (MukulWaval, 2025-10-21 16:29)
- a0f7aae docs: fix badges (MukulWaval, 2025-10-21 16:18)
- 8723ac0 docs: add all docs again (MukulWaval, 2025-10-21 15:47)

### 🚀 Features
- 8a006e6 feat: add pre-commit hooks, update workflows (MukulWaval, 2025-10-21 13:07)

### 🔨 Miscellaneous
- 13d3c05 chore: update version to 0.0.19 and clean up install process in Makefile (MukulWaval, 2025-10-21 15:59)
- b3fdfd5 chore: update version to 0.0.18 and improve documentation across files (MukulWaval, 2025-10-21 15:54)
- 44de168 chore: ensure consistent formatting in .last_commit and VERSION files (MukulWaval, 2025-10-21 14:27)
- 86353d2 chore: update version to 0.1.4, clean up changelog and HISTORY.md, and adjust .gitignore for ruff cache (MukulWaval, 2025-10-21 14:24)
- 42ca215 chore: add ruff cache to .gitignore and update import statement in __init__.py (MukulWaval, 2025-10-21 14:12)

### 🛠 Fixes
- c5e14be fix: correct workflow name casing in test configuration (MukulWaval, 2025-10-21 16:17)
- 0bdcb17 fix: remove outdated assertion from documentation test (MukulWaval, 2025-10-21 16:07)
- 5c9ab17 fix: clean old builds before installation in the install target (MukulWaval, 2025-10-21 16:05)
- c1200c5 fix: clean old builds before installation (MukulWaval, 2025-10-21 16:01)

## Changelog for Version 0.0.19
## Changelog for Version 0.0.20

### 📝 Documentation Updates
- 473cd04 docs: update module documentation (MukulWaval, 2025-10-26 18:33)
- 7dc94fe docs: rewrite to follow conventions (MukulWaval, 2025-10-26 18:07)

### 🚀 Features
- f7e126e feat: add pylintrc configuration (MukulWaval, 2025-10-26 17:00)

### 🔨 Miscellaneous
- e74d8cd chore: remove unused pylintrc files and update pylint disable comments in tests (MukulWaval, 2025-10-26 17:11)
- bfc48f5 chore: update pre-commit configuration (MukulWaval, 2025-10-26 16:23)

### 🔧 CI/CD
- e1b3762 ci: add caching and pre-commit setup to auto-test (MukulWaval, 2025-10-26 16:36)

### 🚀 Release Updates
- 706633a release: bump version to 0.0.20 (MukulWaval, 2025-10-26 18:34)
## Changelog for Version 0.0.20

### 🚀 Features
- ac20fd1 feat: implement Token class (MukulWaval, 2025-10-26 22:44)

### 🔨 Miscellaneous
- a57847a chore: fix end of files (MukulWaval, 2025-10-26 22:49)

### 🔧 CI/CD
- 5044553 ci: update changelog logic (MukulWaval, 2025-10-27 11:35)
## Changelog for Version 0.0.21

### ♻️ Refactoring
- 3e53f22 refactor: update Ruler class to return pairs of rule names and functions (MukulWaval, 2025-12-04 13:07)
- a5ac313 refactor: rename variables to follow Google C++ naming conventions (MukulWaval, 2025-12-03 22:02)
- c63ea6b refactor: specify type for auto variables (MukulWaval, 2025-12-03 21:28)
- fd9d4c0 refactor: rename parameters in BlockRules methods to follow Google C++ naming conventions (MukulWaval, 2025-12-03 20:10)
- 81edeba refactor: encapsulate rule functions within BlockRules and CoreRules classes (MukulWaval, 2025-12-03 19:25)
- 05f1844 refactor: rename everything to folllow google c++ naming conventions (MukulWaval, 2025-12-03 19:01)
- 1595726 refactor: rename rule functions to be consistent (MukulWaval, 2025-12-03 15:07)
- bf7f750 refactor: remove unnecessary NOLINT comments for indentation (MukulWaval, 2025-12-03 15:29)
- 0734614 Merge pull request #9 from aethermark/refactor/deque-for-tokens (Mukul Waval, 2025-12-02 20:29)
- 30a058d refactor: replace std::vector with std::deque for token handling (MukulWaval, 2025-12-02 20:23)
- a27c5a1 refactor: replace accessors with direct member access in Token class (MukulWaval, 2025-12-02 17:56)
- d4398b3 refactor: add TODO comment to address token vector reference invalidation in rules_blockquote (MukulWaval, 2025-12-02 16:46)
- b38f550 refactor: remove TODO comment for SetMapAt tests in Token class (MukulWaval, 2025-12-02 15:00)
- e1a2360 refactor: replace accessors with direct member access in Token class (MukulWaval, 2025-12-02 17:56)
- 22d879c refactor: add TODO comment to address token vector reference invalidation in rules_blockquote (MukulWaval, 2025-12-02 16:46)
- d1fb1ff refactor: remove TODO comment for SetMapAt tests in Token class (MukulWaval, 2025-12-02 15:00)
- 3f8ae30 refactor: Makefile for improved build process for debugging playground (MukulWaval, 2025-11-29 21:49)
- f63c965 refactor: update Token class to use const references (MukulWaval, 2025-10-27 12:25)

### 📝 Documentation Updates
- 987f50a docs: update documentation (MukulWaval, 2025-12-03 14:23)

### 🚀 Features
- 540ebb8 feat: add release trigger for changelog generation workflow (MukulWaval, 2025-12-04 20:44)
- 3025eac feat: update CMake and Makefile to improve test executable configuration and build options (MukulWaval, 2025-12-04 19:03)
- 4d675bd feat: enhance clean target to remove additional build artifacts (MukulWaval, 2025-12-04 18:35)
- 807902e feat: add uninstall target to CMake and update Makefile for uninstall process (MukulWaval, 2025-12-04 18:32)
- 736be47 feat: update test executable configuration and clean up Makefile (MukulWaval, 2025-12-04 18:25)
- ee01f4d feat: port test runner (MukulWaval, 2025-12-04 17:57)
- e977391 feat: add missing files to the last commit (MukulWaval, 2025-12-04 16:59)
- 6f7520a feat: port python build related system (MukulWaval, 2025-12-04 16:58)
- a556380 feat: move isSpace function to utils (MukulWaval, 2025-12-03 11:49)
- 8033fc7 Merge pull request #10 from aethermark/refactor/extract-utils (Mukul Waval, 2025-12-02 20:56)
- da3d329 feat: implement trim utility function and add TODOs for future improvements (MukulWaval, 2025-12-02 20:52)
- e084909 feat: partially implement block parser (MukulWaval, 2025-12-02 10:40)
- c87379f feat: partially implement block parser (MukulWaval, 2025-12-02 10:40)
- 4129453 feat: partially implement core Aethermark functionalities with core parser and rules (MukulWaval, 2025-11-30 12:12)
- c55bcdc feat: add playground functionality with main.cpp and main.py (MukulWaval, 2025-11-23 20:48)
- adaa162 feat: implement Ruler class (MukulWaval, 2025-11-23 20:07)

### 🔧 CI/CD
- 696570b ci: remove changelog workflow (MukulWaval, 2025-12-03 14:45)

### 🎨 Code Style Changes
- e6de114 style: update test suit names to be consistent (MukulWaval, 2025-11-23 21:55)
- 487a531 style: format code and add copyright notice in test files (MukulWaval, 2025-11-23 16:55)

### 🛠 Fixes
- f1b2483 fix: test faliure (MukulWaval, 2025-12-03 19:25)
- f528a02 fix: add cpplint filter argument for whitespace indentation (MukulWaval, 2025-12-03 14:40)
- 62de811 fix: include utils properly (MukulWaval, 2025-12-03 11:53)
- 929164a fix: add TODO comments for block and inline parser implementation (MukulWaval, 2025-12-02 20:34)
- 799732c fix: mapping in blockquote (MukulWaval, 2025-12-02 16:31)
- ec5c3d1 fix: correct type of map in token class (MukulWaval, 2025-12-02 14:59)
- 7d3d716 fix: mapping in blockquote (MukulWaval, 2025-12-02 16:31)
- 68c7d46 fix: correct type of map in token class (MukulWaval, 2025-12-02 14:59)
- 51a13a8 fix: update include patterns in MANIFEST.in to correctly include .tpp files (MukulWaval, 2025-11-30 12:29)
- 3b71eb9 fix: correct classifier entry in setup.cfg (MukulWaval, 2025-11-30 12:25)
