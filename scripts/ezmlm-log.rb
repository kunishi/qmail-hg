#!/usr/bin/ruby

File.open(ARGV[0]) do |file|
  while line = file.gets
    array = line.split(' ')
    top = array.shift
    print Time.at(top.to_i), " ", array.join(' '), "\n"
  end
end
