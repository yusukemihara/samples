def lock
  path = ".lock"
  lock_file = open(path,File::RDWR|File::CREAT)
  begin
    yield if lock_file.flock(File::LOCK_EX)
  ensure
    lock_file.close
  end
end

lock{
  puts "---- A start"
  puts Time.now
  sleep 10
  puts Time.now
  puts "---- A end"
}

lock {
  puts "---- B start"
  puts Time.now
  puts "---- B end"
}
