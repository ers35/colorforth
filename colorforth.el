;; a colorForth major mode

(defvar colorForth-mode-syntax-table (make-syntax-table) "")
(defvar colorForth-mode-abbrev-table (make-abbrev-table) "")

(defface colorForth-mode-define-face
  '((t (:foreground "red")))
  "Face for colorForth mode."
  :group 'colorForth-mode-lock-faces)

(defface colorForth-mode-compile-face
  '((t (:foreground "green4")))
  "Face for colorForth mode."
  :group 'colorForth-mode-lock-faces)

(defface colorForth-mode-execute-face
  '((t (:foreground "yellow4")))
  "Face for colorForth mode."
  :group 'colorForth-mode-lock-faces)

(defface colorForth-mode-comment-face
  '((t (:foreground "grey40")))
  "Face for colorForth mode."
  :group 'colorForth-mode-lock-faces)

(defface colorForth-mode-hide-face
  '((t (:foreground "grey80" :height 0.8)))
  "Face for colorForth mode."
  :group 'colorForth-mode-lock-faces)

(setq colorForth-highlights
  '(("\\(^\\|[ \n\t]\\):[^ \n\t]+"    . (0 'colorForth-mode-define-face t))
     ("\\(^\\|[ \n\t]\\)\\^[^ \n\t]+" . (0 'colorForth-mode-compile-face t))
     ("\\(^\\|[ \n\t]\\)~[^ \n\t]+"   . (0 'colorForth-mode-execute-face t))
     ("\\(^\\|[ \n\t]\\)([^ \n\t]+"   . (0 'colorForth-mode-comment-face t))
     ("\\(^\\|[ \n\t]\\)[:\\^~(]"     . (0 'colorForth-mode-hide-face t))
     ("~\\\\.*$"                      . (0 'colorForth-mode-comment-face t))))

(define-derived-mode colorForth-mode text-mode "colorForth"
  "major mode for editing colorForth language code."
  (setq font-lock-defaults '(colorForth-highlights)))

(setq auto-mode-alist (cons '("\\.fs\\'" . colorForth-mode)
                        auto-mode-alist))
(setq auto-mode-alist (cons '("\\.cf\\'" . colorForth-mode)
                        auto-mode-alist))


(provide 'colorForth-mode)

