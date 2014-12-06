local color = {
  red = "\x1B[01;91m",
  green = "\x1B[01;92m",
  yellow = "\x1B[01;93m",
  white = "\x1B[01;37m",
  clear = "\x1B[0m",
}
while 1 do
  local reading_word = true
  local tib = {}
  local thecolor = color.yellow
  io.write(color.yellow)
  while reading_word do
    local c = io.read(1)
    if c == ":" then
      thecolor = color.red
      io.write(color.red)
    elseif c == "^" then
      thecolor = color.green
      io.write(color.green)
    elseif c == "~" then
      thecolor = color.yellow
      io.write(color.yellow)
    elseif c == "(" then
      thecolor = color.white
      io.write(color.white)
    elseif c == "\n" or c == " " or c == "\t" then
      if #tib == 0 then
        
      else
        reading_word = false
      end
      io.write(c)
    elseif c == nil then
      thecolor = color.clear
      io.write(color.clear)
      return
    else
      table.insert(tib, c)
      io.write(thecolor)
      io.write(c)
    end
  end
end


