" Vim indent file
" Installation: Place this script in the $HOME/.vim/after/indent/ directory
"               as fortran.vim   and use it with Vim > 7.1 and findent:
"               findent.sourceforge.net
"               Author: Willem Vermin wvermin@gmail.com
"               License: fair

" use findent for indenting,  unless use_findent == 0
if !exists("g:use_findent")
   let b:use_findent = 1
else
   let b:use_findent = g:use_findent
endif

" use findent for indenting using indentexpr (see :help indentexpr)
"     unless use_findent_indentexpr == 0
if !exists("g:use_findent_indentexpr")
   let b:use_findent_indentexpr = 1
else
   let b:use_findent_indentexpr = g:use_findent_indentexpr
endif

" The location of findent:
if !exists("g:findent")
   let g:findent = "findent"
endif

if !exists("b:use_findent")
   finish
endif
if !b:use_findent
   finish
endif

" no interesting side-effects caused by settings in FINDENT_FLAGS:
let $FINDENT_FLAGS = ""

if !exists("b:use_findent_indentexpr")
   let b:use_findent_indentexpr = 1
endif

autocmd BufEnter * unlet! g:fortran_free_source g:fortran_fixed_source
autocmd BufEnter * unlet! b:fortran_free_source b:fortran_fixed_source

" g:findent is probably set in .vimrc
" if not: skip all
if !exists("g:findent")
   finish
endif

" get fortran format from extension ext
" return "free", "fixed" or "unknown"
function! Get_format_from_extension(ext)
   if a:ext =~? '^f$\|^for$\|^fpp$\|^ftn$\|^fortran$\|^f77$'
      let format = "fixed"
   elseif a:ext =~? '^f90$\|^f95$\|^f03$\|^f08$'
      let format = "free"
   else
      let format = "unknown"
   endif
   return format
endfunction

" return string that represents external command to determine
" if input is free or fixed format.
" If no suitable command is found, return ""
" The output of this external command must be "free" or "fixed"
function! Findent_get_freefixed()
   let indentparmsq = ' -q'
   let f = g:findent.indentparmsq
   if strpart(system(f," continue"),0,4) != "free"
      let f = ""
   endif
   return f
endfunction

" Return external command to get the indent of the last line of the input
" Return "" if no such command can be found
" side effect: s:findent_getindent is the command tried
function! Findent_get_getindent()
   let getindent=g:findent.' -Ia -lastindent -i'.b:fortran_format.' '.b:findent_flags
   let s:findent_getindent = getindent
   if system(getindent,'      continue') == 6
      return getindent
   else
      return ""
   endif
endfunction

" Return external command that indents the input
" Return "" if no such command can be found
" side effect: s:fortran_indentprog is the command tried
function! Findent_get_indentprog()
   " first a test:
   let indentparms = ' -Ia -i'.b:fortran_format.' '.b:findent_flags
   let indentprog = g:findent.indentparms
   let s:fortran_indentprog = indentprog
   if strpart(system(indentprog,"continue"),0,8) == "continue"
      return indentprog
   else
      return ""
   endif
endfunction

" Returns the indentation of the current line
function! Findent_getindent()
   if b:findent_use_whole_buffer
      " use whole buffer up to current line to determine indent
      let startline = 1
   else
      let maxlines = 10*&lines
      " use at most 'maxlines' previous lines to determine indent
      let s = max([v:lnum-maxlines,1])
      "let startline = system(g:findent." -lastusable -i".b:fortran_format,join(getline(1,v:lnum-1),"\n"))
      let startline = s - 1 + system(g:findent." -lastusable -i".b:fortran_format,join(getline(s,v:lnum-1),"\n"))
   endif
   let startline = max([1,startline])
   let getindent = Findent_get_getindent()
   let lnum = prevnonblank(v:lnum)
   let ind  = system(getindent, join(getline(startline,lnum),"\n"))
   return ind
endfunction

function! Findent_set_flags()
   if !b:have_findent && !b:have_findent_getindent
      return
   endif
   let old = b:findent_flags
   let b:findent_flags = input("findent flags:",b:findent_flags)
   if b:have_findent
      if Findent_get_indentprog() != ""
	 call Findent_set_indentprog()
      else
	 let x = input( "Cannot use flags '".b:findent_flags."' Will use '".old."'")
	 let b:findent_flags = old
	 return
      endif
   endif
   if b:have_findent_getindent
      if Findent_get_getindent() != ""
	 call Findent_set_getindent() 
      else
	 let x = input( "Cannot use flags '".b:findent_flags."' Will use '".old."'")
	 let b:findent_flags = old
	 return
      endif
   endif
endfunction

function! Findent_set_indentprog()
   let p = Findent_get_indentprog()
   if p == ""
      echomsg "Warning '".s:fortran_indentprog."' not usable as equalprg"
      echomsg "Using default for equalprg"
      setlocal equalprg=
      let b:have_findent = 0
   else
      execute 'setlocal equalprg='.substitute(p,' ','\\ ','g')
      let b:have_findent = 1
   endif
endfunction

function! Findent_set_getindent()
   if Findent_get_getindent() == ""
      echomsg "Warning '".s:findent_getindent."' not usable as indentexpr"
      echomsg "Using default for indentexpr"
      setlocal indentexpr=
      let b:have_findent_getindent = 0
   else
      setlocal indentexpr=Findent_getindent()
      setlocal indentkeys=*<Return>,*<Up>,*<Down>,*<Esc>,!^F,o,O,0#
      " fixed format: if first character is tab, indent the line
      if b:fortran_format == "fixed"
	 setlocal indentkeys+=!0<Tab>
      endif
      let b:have_findent_getindent = 1
   endif
endfunction

function! Findent_use_wb_toggle()
   if b:use_findent_indentexpr
      let b:findent_use_whole_buffer = !b:findent_use_whole_buffer
      if b:findent_use_whole_buffer 
	 let p = "ON"
      else
	 let p = "OFF"
      endif
      echomsg "use whole buffer = ".p
   else
      echomsg "not using findent for indentexpr"
   endif
endfunction

function! Get_free_or_fixed_default()
   if exists("b:fortran_fixed_source")
      if b:fortran_fixed_source
	 return "fixed"
      endif
   endif
   if exists("b:fortran_free_source")
      if b:fortran_free_source
	 return "free"
      endif
   endif
   return "fixed"
endfunction

"=========================================================================

if exists("g:findent_flags")
   let b:findent_flags = g:findent_flags
endif

if !exists("b:findent_flags")
   let b:findent_flags = "-i".&shiftwidth
endif

if !exists("b:findent_use_whole_buffer")
   let b:findent_use_whole_buffer = 0
endif

if !exists("g:findent")
   let g:findent = "/usr/bin/findent"
endif

" determine fixed or free or unknown based on suffix

let b:fortran_format = Get_format_from_extension(expand('%:e'))

" if format is unknown, try to get it from the source
if b:fortran_format == "unknown"
   " determine command to determine free or fixed
   let b:findent_freefixed = Findent_get_freefixed()
   if b:findent_freefixed == ""
      let b:fortran_format = Get_free_or_fixed_default()
      echomsg "Cannot determine format using findent, will use" b:fortran_format
   else
      let r=system(b:findent_freefixed,join(getline(1,10000),"\n"))
      if r =~ "free"
	 let b:fortran_format = "free"
      else
	 let b:fortran_format = "fixed"
      endif
   endif
endif

if b:fortran_format == "free"
   let g:fortran_free_source = 1
   let b:fortran_free_source = 1
   let b:fortran_fixed_source = 0
   "setlocal colorcolumn=133
   if exists("g:findent_setcolumns")
      if(g:findent_setcolumns)
	 setlocal numberwidth=6
	 if &columns < 132+&numberwidth
	    let &columns=132+&numberwidth
	 endif
      endif
   endif
else
   let g:fortran_fixed_source = 1
   let b:fortran_fixed_source = 1
   let b:fortran_free_source = 0
   "setlocal colorcolumn=6,73
endif

call Findent_set_indentprog()

if b:use_findent_indentexpr
   call Findent_set_getindent()
endif

