"
" All system-wide defaults are set in $VIMRUNTIME/debian.vim and sourced by
" the call to :runtime you can find below.  If you wish to change any of those
" settings, you should do it in this file (/etc/vim/vimrc), since debian.vim
" will be overwritten everytime an upgrade of the vim packages is performed.
" It is recommended to make changes after sourcing debian.vim since it alters
" the value of the 'compatible' option.

" This line should not be removed as it ensures that various options are
" properly set to work with the Vim-related packages available in Debian.
runtime! debian.vim

" Uncomment the next line to make Vim more Vi-compatible
" NOTE: debian.vim sets 'nocompatible'.  Setting 'compatible' changes numerous
" options, so any other options should be set AFTER setting 'compatible'.
"set compatible

" Vim5 and later versions support syntax highlighting. Uncommenting the next
" line enables syntax highlighting by default.
if has("syntax")
  syntax on
endif

" If using a dark background within the editing area and syntax highlighting
" turn on this option as well
"set background=dark

" Uncomment the following to have Vim jump to the last position when
" reopening a file
"if has("autocmd")
"  au BufReadPost * if line("'\"") > 1 && line("'\"") <= line("$") | exe "normal! g'\"" | endif
"endif

" Uncomment the following to have Vim load indentation rules and plugins
" according to the detected filetype.
"if has("autocmd")
"  filetype plugin indent on
"endif

" The following are commented out as they cause vim to behave a lot
" differently from regular Vi. They are highly recommended though.
"set showcmd		" Show (partial) command in status line.
"set showmatch		" Show matching brackets.
"set ignorecase		" Do case insensitive matching
"set smartcase		" Do smart case matching
"set incsearch		" Incremental search
"set autowrite		" Automatically save before commands like :next and :make
"set hidden		" Hide buffers when they are abandoned
"set mouse=a		" Enable mouse usage (all modes)

" Source a global configuration file if available
" if filereadable("/etc/vim/vimrc.local")
  " source /etc/vim/vimrc.local
" endif






inoremap jj <esc>
"set nocompatible
set nu
set syntax=on
set tabstop=4
set shiftwidth=4
set expandtab
set autoindent
set ruler
set cursorline
set nobackup
set aw
set cin
filetype on
set fdm=marker
" set fdm=syntax
set nofoldenable

" :verbose map <TAB>

set showmatch
set ruler
set smartindent
set incsearch

set tags=~/path/tags
set tags=tags
set autochdir

set fileencoding=utf-8
"set termencoding=utf-8
"set encoding=prc

" Allow saving of files as sudo when I forget to start with vim using sudo.
cmap w!! w !sudo tee > /dev/null %


set nocompatible               
filetype off                   

" set rtp += ~/.vim/bundle/vundle/ 
set rtp+=~/.vim/bundle/Vundle.vim
" call vundle#rc()
call vundle#begin()
 
" let Vundle manage Vundle
" required! 
Plugin 'VundleVim/Vundle.vim'
" Define bundles via Github repos
Plugin 'scrooloose/nerdtree'
Plugin 'scrooloose/nerdcommenter'
Plugin 'scrooloose/syntastic'
Plugin 'Lokaltog/vim-powerline'
Plugin 'a.vim'
Plugin 'vim-scripts/ctags.vim'
Plugin 'godlygeek/tabular'
Plugin 'msanders/snipmate.vim'
Plugin 'iamcco/mathjax-support-for-mkdp'
Plugin 'iamcco/markdown-preview.vim'
" Plugin 'tamlok/vim-markdown'
Plugin 'plasticboy/vim-markdown'
" Plugin 'JamshedVesuna/vim-markdown-preview'

" Plugin 'gabrielelana/vim-markdown' 
" Plugin 'tamlok/vim-markdown'
 " filetype on
call vundle#end()
filetype plugin indent on







"nmap <F5> :NERDTreeToggle<cr>
nmap <F5> :NERDTreeToggle<cr>
" 切换头文件和源文件
nnoremap <F12> :A<CR>

let g:syntastic_always_populate_loc_list = 1
let g:syntastic_check_on_open = 1
let g:syntastic_auto_jump = 1
"let g:syntastic_auto_jump = 3
let g:syntastic_cpp_checkers=['gcc']
let g:syntastic_cpp_compiler='gcc'
let g:syntastic_cpp_compiler_options='-std=c++11'


" for powerline
set laststatus=2
let g:Powerline_symbols='unicode'

" NerdCommonter
let mapleader=","
let g:NERDSpaceDelims=1
filetype plugin on

" for tabular
if !exists(":Tabularize")
    nmap <Leader>a= :Tabularize /=<CR>
    vmap <Leader>a= :Tabularize /=<CR>
    nmap <Leader>a: :Tabularize /:\zs<CR>
    vmap <Leader>a: :Tabularize /:\zs<CR>
endif

" for space



"设置= + - * 前后自动空格
""设置，后面自动添加空格
" if !exists("g:equ")
if !exists("g:equ")
    :inoremap = <c-r>=EqualSign('=')<CR>
    :inoremap + <c-r>=EqualSign('+')<CR>
    :inoremap - <c-r>=EqualSign('-')<CR>
    :inoremap * <c-r>=EqualSign('*')<CR>
    :inoremap / <c-r>=EqualSign('/')<CR>
    :inoremap { <c-r>=EqualSign('{')<CR>
    " :inoremap != <c-r>=EqualSign('!=')<CR>
    " :inoremap == <c-r>=EqualSign('==')<CR>
    " :inoremap && <c-r>=EqualSign('&&')<CR>
    " :inoremap > <c-r>=EqualSign('>')<CR>
    " :inoremap < <c-r>=EqualSign('<')<CR>
    " :inoremap << <c-r>=EqualSign('<<')<CR>
    " :inoremap >> <c-r>=EqualSign('>>')<CR>
    :inoremap , ,<space>
    " :inoremap . .<space>
    :inoremap ; ;<space>
endif


function! EqualSign(char)
    if a:char  =~ '='  && getline('.') =~ ".*("
    return a:char
    endif 
    let ex1 = getline('.')[col('.') - 3]
    let ex2 = getline('.')[col('.') - 2]


    "if ex1 =~ "[-=+><>\/\*]"
    "if ex1 =~ "[-==+>}\/\*]"
    if ex1 =~ "[!=-==+>}\/\*]"
    if ex2 !~ "\s"
    return "\<ESC>i".a:char."\<SPACE>"
    else
    return "\<ESC>xa".a:char."\<SPACE>"
    endif 
    else
    if ex2 !~ "\s"
    return "\<SPACE>".a:char."\<SPACE>\<ESC>a"
    else
    return a:char."\<SPACE>\<ESC>a"
    endif 
    endif

endfunction

" for header
" autocmd BufNewFile *.php,*.js,*.cpp,*.c,*.py,*.h  exe ":call SetComment()"
" autocmd BufNewFile *.php,*.cpp,*.c,*.py,*.h  exe ":call SetComment()"
autocmd BufNewFile *.php,*.cpp,*.c,*.h  exe ":call SetComment()"

func SetComment()
    if expand("%:e")=='php'
        call setline(1,"<?php")
    " elseif expand("%:e")=='js'
        " call setline(1,"//JavaScript file")
    elseif expand("%:e")=='cpp'
        call setline(1,"//C++ file")
    elseif expand("%:e")=='c'
        call setline(1,"//C file")
    elseif expand("%:e")=='py'
        call setline(1,"##Python file")
    endif
    if &filetype=='python'
        " call append(1,'#****************************************************')
        call append(1,"'''")
        call append(2,'#')
        call append(3,'# Filename: '.expand("%"))
        call append(4,'#')
        call append(5,'# Author: feihong -- lhfly520@gmail.com')
        call append(6,'# Description:---')
        call append(7,'# Create:        '.strftime("%Y-%m-%d %H:%M:%S"))
        call append(8,'# Last Modified: '.strftime("%Y-%m-%d %H:%M:%S"))
        call append(9,'#****************************************************')
        " call append(10,'')
        call append(10,"'''")
    endif

    if &filetype=='cpp'
        call append(1,'/****************************************************')
        call append(2,'*')
        call append(3,'* Filename: '.expand("%"))
        call append(4,'*')
        call append(5,'* Author: feihong -- lhfly520@gmail.com')
        call append(6,'* Description:---')
        call append(7,'* Create:        '.strftime("%Y-%m-%d %H:%M:%S"))
        call append(8,'* Last Modified: '.strftime("%Y-%m-%d %H:%M:%S"))
        call append(9,'*****************************************************/')
        call append(10,'')
    endif

    if &filetype=='c'
        call append(1,'/****************************************************')
        call append(2,'*')
        call append(3,'* Filename: '.expand("%"))
        call append(4,'*')
        call append(5,'* Author: feihong -- lhfly520@gmail.com')
        call append(6,'* Description:---')
        call append(7,'* Create:        '.strftime("%Y-%m-%d %H:%M:%S"))
        call append(8,'* Last Modified: '.strftime("%Y-%m-%d %H:%M:%S"))
        call append(9,'*****************************************************/')
        call append(10,'')
    endif

endfunc

map <F2> :call SetComment()<CR>:10<CR>o
" SET Comment END
" modified time of a file
function SetLastModifiedTime(lineno)
    let modif_time = strftime("%Y-%m-%d %H:%M:%S")
    if a:lineno == "-1"
        let line = getline(6)
    else
        let line = getline(a:lineno)
    endif    
    if line =~ '^////\sLast Modified'
        let line = substitute( line,':\s\+.*\d\{4\}', ':'.modif_time, "" )
    else
        let line = '* Last Modified: '.modif_time
    endif
    if a:lineno == "-1"
        call setline(9, line)
    else
        call append(a:lineno, line)
    endif
endfunction

" map the SetLastModifiedTime command automatically
" au BufWrite *.js,*php,*.py,*.java,*.cpp,*.c,*.h  call SetLastModifiedTime(-1) 
" au BufWrite *php,*.py,*.java,*.cpp,*.c,*.h  call SetLastModifiedTime(-1) 
au BufWrite *php,*.java,*.cpp,*.c,*.h  call SetLastModifiedTime(-1) 

"预防手误的杀招
nnoremap ： :
cnoremap Q! q!
cnoremap Q q
command Wq wq
command Q q
command WQ wq
command W w

" for markdown preview
" 普通模式
" nmap <silent> <F7> <Plug>MarkdownPreview        
" nmap <F7> <Plug>MarkdownPreview        
" " 插入模式
" imap <silent> <F8> <Plug>MarkdownPreview        
" " 普通模式
" " nmap <silent> <F9> <Plug>MarkdownPreviewStop    
" nmap <silent> <F8> <Plug>StopMarkdownPreview
" nmap <F8> <Plug>StopMarkdownPreview
" " 插入模式
" imap <silent> <F9> <Plug>StopMarkdownPreview


" let g:mkdp_auto_start=1
" let g:mkdp_auto_open=1
let g:mkdp_command_for_global=1
