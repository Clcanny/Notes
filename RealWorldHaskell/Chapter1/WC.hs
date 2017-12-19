main = interact charCount
    where lineCount input = show (length (lines input)) ++ "\n"
          wordCount input = show (length (words input)) ++ "\n"
          charCount input = show (length input) ++ "\n"
