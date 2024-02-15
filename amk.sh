#!/bin/bash

sudo yum install -y mc gcc git

sudo dnf install 'dnf-command(config-manager)'
sudo dnf config-manager --add-repo https://cli.github.com/packages/rpm/gh-cli.repo
sudo dnf install gh

gh auth login
#sudo install -y gcc
#comment
