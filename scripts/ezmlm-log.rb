#!/usr/bin/ruby

File.open(ARGV[0]) do |file|
  while line = file.gets
    array = line.split(' ')
    print Time.at(array.shift.to_i), " ", array.join(' '), "\n"
  end
end
