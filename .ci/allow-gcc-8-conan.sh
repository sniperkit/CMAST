#!/usr/bin/env bash
git clone https://github.com/Gallore/yaml_cli
pushd yaml_cli
pip install . --user
popd
yaml_cli -f ~/.conan/settings.yml -s compiler:gcc:version '["8"]'
