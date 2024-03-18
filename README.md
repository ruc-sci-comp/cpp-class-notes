# cpp-class-notes

This repository contains all of the notes distributed throughout the semester. This repository will be updated with new material before each class.

## Contents

* `Class-01` - Introduction to C++ & Scientific Computing
* `Class-02` - Variables and Control Structures
* `Class-03` - Operators, For-Loops, Functions, Introduction to Simulations
* `Class-04` - More on Functions, Introduction to STL, Introduction to Kinematics
* `Class-05` - Standard Template Library (STL), fmt
* `Class-06` - Lambdas, Scope, Constness
* `Class-07` - Classes, Initialization, Memory Optimization
* `Class-08` - Introduction to Randomness
* `Class-09` - ...
* `Class-10` - ...
* `Class-11` - ...
* `Class-12` - ...
* `Class-13` - ...
* `Class-14` - ...

## Usage

These instructions apply to all respositories in this class.

### GitHub Codespaces (simple)

Throughout this course GitHub Codespaces will always be available. A codespace is an cloud-based development environment, preconfigured using our course environment [`turtleshell`](https://github.com/lnk2past/turtleshell). Codespaces effectively provide VSCode in your web browser.

From any repository in this course you can simply click the green "Code" button, select the "Codespaces" tab, and then click the "Create codespace on main" button. This will take a few moments to download all of the necessary resources before opening VSCode.

#### Managing Codespaces

Codespaces are *free*, but because they are free, their resources are limited. Your unused codespaces will automatically expire, and you are limited in the number of codespaces you can actively own. You will inevitably need to delete old codespaces as we build a small collection of them throughout the semester. You can do so [here](https://github.com/codespaces).

#### Saving Progress & Files

While every codespace is attached to GitHub, you are responsible for *synchronizing* them. This is done through the UI or through a series of `git` commands that will `stage`, `commit`, and `push` the files to GitHub.

You can leave a codespace and allow it to shutdown automatically (shutting down is different than expiration!), but you are encourage to synchronize your code as much as you can; there is no penalty for synchronizing your code many time. Any code and changes you make in a codespace can be retrieved *so long as the codespace has not been deleted!*.

### Devcontainers (advanced)

For those with their own laptop (or PC at home) with a sufficient operating system can also decide to launch a *devcontainer*. This in essence is the exact same as a codespace, but instead the environment is running on your laptop rather than in the cloud. This is not too complicated to setup, but there are a few pieces of software you will need to install yourself.

Development using a local devcontainer will be much smoother, and our code will run much faster. Once you have all of the tools listed below installed, you will download a repository (via GitHub Desktop/git), open it in VSCode, and re-open the repository in a devcontainer.

#### Docker

First and foremost, if you cannot install [`Docker`](https://www.docker.com/get-started/) then you cannot proceed with running devcontainers. There are somewhat strict requirements on operating systems, and so if your system is too old then you cannot install Docker. The installation process may require a restart of your computer, and may require additional updates made to operating system.

Docker is responsible for running the containerization (similar to virtualization) of the development environment.

#### VSCode

Install [`VSCode`](https://code.visualstudio.com/), along with the following extension:

* `ms-vscode-remote.vscode-remote-extensionpack`

VSCode is a fully featured text editor with many powerful extensions that bring it from editor to integrated development environment.

#### GitHub Desktop

Install [`GitHub Desktop`](https://desktop.github.com/). While not strictly necessary, this provides you with an easy-to-use UI for downloading repositories and submitting files back to GitHub. You will then need to configure SSH keys, [as detailed here](https://docs.github.com/en/authentication/connecting-to-github-with-ssh/generating-a-new-ssh-key-and-adding-it-to-the-ssh-agent).

