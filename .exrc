if &cp | set nocp | endif
let s:cpo_save=&cpo
set cpo&vim
imap <silent> <expr> <F5> (pumvisible() ? "\<Plug>LookupFileCE" : "")."\\<Plug>LookupFile"
inoremap <Plug>LookupFileCE 
nmap  :bottomExplorerWindow
nmap  :FirstExplorerWindow
nnoremap <silent> w :CCTreeWindowToggle
nnoremap <silent> y :CCTreeWindowSaveCopy
nmap  
nmap d :cs find d =expand("<cword>")
nmap i :cs find i =expand("<cword>")
nmap f :cs find f =expand("<cword>")
nmap e :cs find e =expand("<cword>")
nmap t :cs find t =expand("<cword>")
nmap c :cs find c =expand("<cword>")
nmap g :cs find g =expand("<cword>")
nmap s :cs find s =expand("<cword>")
xnoremap <silent>  y:call g:vimim_visual()
nnoremap <silent>  i=g:vimim_chinese()
nmap ,caL <Plug>CalendarH
nmap ,cal <Plug>CalendarV
nmap ,ihn :IHN
nmap ,is :IHS:A
nmap ,ih :IHS
vmap <silent> ,hr <Plug>MarkRegex
nmap <silent> ,hr <Plug>MarkRegex
vmap <silent> ,hh <Plug>MarkClear
nmap <silent> ,hh <Plug>MarkClear
vmap <silent> ,hl <Plug>MarkSet
nmap <silent> ,hl <Plug>MarkSet
map ,ts :set tags=tags :cs add cscope.out
nmap <silent> ,lw :LUWalk
nmap <silent> ,ll :LUBufs
nmap <silent> ,lk :LUTags
nmap <silent> ,wm :WMToggle
nmap <silent> ,fe :Sexplore!
map ,sm :source Session.vim :rviminfo vim.viminfo
map ,mk :mksession! :wviminfo vim.viminfo
map <silent> ,ee :e ~/.vimrc
map ,ss :source ~/.vimrc
nmap gx <Plug>NetrwBrowseX
xmap <silent> gi 
nnoremap <silent> gi a=g:vimim_gi()
nnoremap <silent> n :call g:vimim_search()n
nnoremap <silent> <Plug>NetrwBrowseX :call netrw#NetrwBrowseX(expand("<cWORD>"),0)
nmap <silent> <F5> <Plug>LookupFile
nnoremap <silent> <Plug>CalendarH :cal calendar#show(1)
nnoremap <silent> <Plug>CalendarV :cal calendar#show(0)
map <silent> <F8> :TlistToggle
imap  
inoremap <silent>  =g:vimim_onekey()
inoremap  =g:vimim_chinese()
imap ,ihn :IHN
imap ,is :IHS:A
imap ,ih :IHS
let &cpo=s:cpo_save
unlet s:cpo_save
set autoindent
set background=dark
set backspace=2
set cindent
set cscopeprg=/usr/bin/cscope
set cscopetag
set cscopeverbose
set fileencodings=utf-8,gbk,iso-8859-1
set guicursor=n-v-c:block,o:hor50,i-ci:hor15,r-cr:hor30,sm:block,a:blinkon0
set helplang=cn
set history=50
set hlsearch
set laststatus=2
set ruler
set shiftwidth=4
set showmatch
set softtabstop=4
set tabstop=4
set tags=tags
set termencoding=utf-8
set viminfo='20,\"50
set nowrapscan
" vim: set ft=vim :
