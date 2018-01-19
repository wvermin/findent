;;;; this file contains an interface to use findent within emacs.
;;;; the C-M-q command ("indent function") is redefined to
;;;; indent the whole buffer.
; how to call findent for a fixed format Fortran source:
(defvar fortran-findent-command 
  "findent -ifixed -Ia"
  "findent command for fixed format fortran source")
;
; how to call findent for a free format Fortran source:
(defvar f90-findent-command 
  "findent -ifree -Ia"
  "findent command for free format fortran source")
;
;;;; define function findent-indent-buffer: it calls the program
;;;; findent with the whole buffer as input and output
(defun findent-indent-buffer()
  " 
  Function that uses findent to indent the whole buffer.
  The call to findent must be provided in the string findent-command,
    for example (setq findent-command \"findent -ifixed -Ia\")
  Findent supports Fortran-2008, free and fixed format;
    indents correctly DO statements that share a common label;
    is in general not confused by #if, #ifdef or #else preprocessor
    statements.
  "
  (interactive)
  (let (
	(lines (count-lines (point-min) (point-max)))
	(lpos (line-number-at-pos))
	(first-window-line)
	(offset)
	(linelength)
	(pos (point))
	)
    ; this function tries to restore cursor and window
    ; position after indenting, hence the extra code.
    ; If somebody knows something better ...
    (beginning-of-line)
    (end-of-line)
    (skip-chars-backward "[:blank:]")
    (setq offset (- (point) pos))
    (if (< offset 0) (setq offset 0))
    (move-to-window-line 0)
    (setq first-window-line (line-number-at-pos))
    (message "indenting buffer ..." )
    (shell-command-on-region (point-min) (point-max) findent-command 1 1 )
    (goto-line lpos)
    (redisplay)
    (move-to-window-line 0)
    (scroll-down (- (line-number-at-pos) first-window-line))
    (goto-line lpos)
    (setq pos (point))
    (end-of-line)
    (setq linelength (- (point) pos))
    (if (< offset linelength)
      (backward-char offset)
      (beginning-of-line))
    (message "indenting buffer ... %d lines indented" lines)
    )
  )
;
(add-hook 'fortran-mode-hook (lambda() (setq findent-command fortran-findent-command)))
(add-hook 'f90-mode-hook     (lambda() (setq findent-command f90-findent-command)))
(add-hook 'fortran-mode-hook (lambda() (local-set-key "\C-\M-q" 'findent-indent-buffer)))
(add-hook 'f90-mode-hook     (lambda() (local-set-key "\C-\M-q" 'findent-indent-buffer)))
