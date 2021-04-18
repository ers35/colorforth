;; a colorForth major mode

(defvar colorForth-mode-syntax-table (make-syntax-table) "")
(defvar colorForth-mode-abbrev-table (make-abbrev-table) "")

(defface colorForth-mode-define-face
  '((t (:foreground "red" :extend t)))
  "Face for colorForth mode."
  :group 'colorForth-mode-lock-faces)

(defface colorForth-mode-compile-face
  '((t (:foreground "green4" :extend t)))
  "Face for colorForth mode."
  :group 'colorForth-mode-lock-faces)

(defface colorForth-mode-execute-face
  '((t (:foreground "yellow4" :extend t)))
  "Face for colorForth mode."
  :group 'colorForth-mode-lock-faces)

(defface colorForth-mode-comment-face
  '((t (:foreground "grey40" :extend t)))
  "Face for colorForth mode."
  :group 'colorForth-mode-lock-faces)

(defface colorForth-mode-tick-face
  '((t (:foreground "blue" :extend t)))
  "Face for colorForth mode."
  :group 'colorForth-mode-lock-faces)

(setq colorForth-highlights
  '((":[^:~^(']+" . (0 'colorForth-mode-define-face t))
     ("\\^[^:~^(']+" . (0 'colorForth-mode-compile-face t))
     ("~[^:~^(']+" . (0 'colorForth-mode-execute-face t))
     ("([^:~^(']+" . (0 'colorForth-mode-comment-face t))
     ("'[^:~^(']+" . (0 'colorForth-mode-tick-face t))))

(define-derived-mode colorForth-mode text-mode "colorForth"
  "major mode for editing colorForth language code."
  (set (make-local-variable 'font-lock-multiline) t)
  (setq font-lock-defaults '(colorForth-highlights)))

(setq auto-mode-alist (cons '("\\.cf\\'" . colorForth-mode)
                        auto-mode-alist))


(provide 'colorForth-mode)
