# If you come from bash you might have to change your $PATH.
# export PATH=$HOME/bin:/usr/local/bin:$PATH

# Path to your oh-my-zsh installation.
export ZSH=$HOME/.oh-my-zsh

# Set name of the theme to load. Optionally, if you set this to "random"
# it'll load a random theme each time that oh-my-zsh is loaded.
# See https://github.com/robbyrussell/oh-my-zsh/wiki/Themes
#ZSH_THEME="robbyrussell"
ZSH_THEME="agnoster"

# Uncomment the following line to use case-sensitive completion.
# CASE_SENSITIVE="true"

# Uncomment the following line to use hyphen-insensitive completion. Case
# sensitive completion must be off. _ and - will be interchangeable.
# HYPHEN_INSENSITIVE="true"

# Uncomment the following line to disable bi-weekly auto-update checks.
 DISABLE_AUTO_UPDATE="true"

# Uncomment the following line to change how often to auto-update (in days).
# export UPDATE_ZSH_DAYS=13

# Uncomment the following line to disable colors in ls.
# DISABLE_LS_COLORS="true"

# Uncomment the following line to disable auto-setting terminal title.
# DISABLE_AUTO_TITLE="true"

# Uncomment the following line to enable command auto-correction.
# ENABLE_CORRECTION="true"

# Uncomment the following line to display red dots whilst waiting for completion.
# COMPLETION_WAITING_DOTS="true"

# Uncomment the following line if you want to disable marking untracked files
# under VCS as dirty. This makes repository status check for large repositories
# much, much faster.
# DISABLE_UNTRACKED_FILES_DIRTY="true"

# Uncomment the following line if you want to change the command execution time
# stamp shown in the history command output.
# The optional three formats: "mm/dd/yyyy"|"dd.mm.yyyy"|"yyyy-mm-dd"
# HIST_STAMPS="mm/dd/yyyy"

# Would you like to use another custom folder than $ZSH/custom?
# ZSH_CUSTOM=/path/to/new-custom-folder

# Which plugins would you like to load? (plugins can be found in ~/.oh-my-zsh/plugins/*)
# Custom plugins may be added to ~/.oh-my-zsh/custom/plugins/
# Example format: plugins=(rails git textmate ruby lighthouse)
# Add wisely, as too many plugins slow down shell startup.
plugins=(git)

source $ZSH/oh-my-zsh.sh

# User configuration

# export MANPATH="/usr/local/man:$MANPATH"

# You may need to manually set your language environment
# export LANG=en_US.UTF-8

# Preferred editor for local and remote sessions
# if [[ -n $SSH_CONNECTION ]]; then
#   export EDITOR='vim'
# else
#   export EDITOR='mvim'
# fi

# Compilation flags
# export ARCHFLAGS="-arch x86_64"

# ssh
# export SSH_KEY_PATH="~/.ssh/rsa_id"

# Set personal aliases, overriding those provided by oh-my-zsh libs,
# plugins, and themes. Aliases can be placed here, though oh-my-zsh
# users are encouraged to define aliases within the ZSH_CUSTOM folder.
# For a full list of active aliases, run `alias`.
#
# Example aliases
# alias zshconfig="mate ~/.zshrc"
# alias ohmyzsh="mate ~/.oh-my-zsh"
bindkey -v
bindkey -M viins 'jj' vi-cmd-mode



# if running bash
if [ -n "$BASH_VERSION" ]; then
    # include .bashrc if it exists
    if [ -f "$HOME/.bashrc" ]; then
	. "$HOME/.bashrc"
    fi
fi


# export PATH="/usr/local/lib/python3.5/dist-packages:$PATH"
# set PATH so it includes user's private bin directories
PATH="$HOME/bin:$HOME/.local/bin:$PATH"
# # added by Anaconda2 4.4.0 installer
# export PATH="/home/feihong/anaconda2/bin:$PATH"
# export PATH="/home/feihong/anaconda2/lib:$PATH"
# # added by Anaconda3 4.4.0 installer
# export PATH="/home/feihong/anaconda3/bin:$PATH"
# export PATH="/home/feihong/anaconda3/lib:$PATH"
source /opt/ros/kinetic/setup.zsh

# # alias python3=python3
# # alias python=python2
source /home/feihong/catkin_ws/devel/setup.zsh


# export PYTHONPATH="/usr/local/lib/python3.5/dist-packages/cv2:$PYTHONPATH"
# export PYTHONPATH="/usr/local/lib/python3.5/dist-packages:$PYTHONPATH"
# # export PYTHONPATH="/usr/local/lib/python2.7/dist-packages:$PYTHONPATH"
# export PYTHONPATH="/home/feihong/anaconda2/lib:$PYTHONPATH"
export PYTHONPATH="/home/feihong/anaconda3/lib/python3.5/site-packages:$PYTHONPATH"
export PATH="/home/feihong/anaconda3/bin:$PATH"
export PYTHONPATH=$PYTHONPATH:`pwd`:`pwd`/slim
export GOPATH=/usr/local/go  
export PATH=$GOPATH/bin:$PATH 

export GOROOT=/usr/local/go  
export GOPATH=$PATH:$GOROOT/bin
