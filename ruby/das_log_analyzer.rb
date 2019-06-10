require 'pp'
require 'date'

data = <<EOF
I, [2011-04-26T02:00:01.831610 #2603]  INFO -- : ==== start prepare_analyze_for_infection
I, [2011-04-26T02:00:02.106674 #2603]  INFO -- : [SUCCESS] no download data
I, [2011-04-26T02:00:02.366868 #2603]  INFO -- : [SUCCESS] no download data
I, [2011-04-26T02:15:01.301782 #2643]  INFO -- : ==== start prepare_analyze_for_infection
I, [2011-04-26T02:15:01.532960 #2643]  INFO -- : [SUCCESS] no download data
I, [2011-04-26T02:15:01.752121 #2643]  INFO -- : [SUCCESS] no download data
I, [2011-04-26T02:30:01.689136 #2689]  INFO -- : ==== start prepare_analyze_for_infection
I, [2011-04-26T02:30:01.954435 #2689]  INFO -- : [SUCCESS] no download data
I, [2011-04-26T02:30:02.188822 #2689]  INFO -- : [SUCCESS] download file : data_2011042602300216.tar
I, [2011-04-26T02:30:02.218116 #2689]  INFO -- : begin insert 3 record
I, [2011-04-26T02:30:26.015886 #2689]  INFO -- : end
EOF

def analyze(file)
  lines = File.read(file).split("\n").map {|l| l.chomp}
  results = []
  r = {}
  lines.each {|l|
    if /([IE]), \[(.*?) #\d+\].* -- : (.*)/ =~ l
      epoch = DateTime.parse($2).strftime("%s").to_i
      level = $1
      datetime = $2
      log = $3
      case level
      when 'I'
        if /====/ =~ log
          if r[:t1]
            results << r
          end
          r = {:t1 => epoch,:count => 0,:datetime => datetime}
        elsif /\[SUCCESS\] download file/ =~ log
          r[:t2] = epoch
        elsif /^begin insert (\d+) record/ =~ log
          r[:count] = r[:count].to_i + $1.to_i
        else
          r[:t3] = epoch
        end
      end
    end
  }
  puts "+----------------------------+-------+-------+-------+-------+"
  puts "| date                       | csv   | total | dl    | db    |"
  puts "+----------------------------+-------+-------+-------+-------+"
  results.each{|r|
    if r[:t2]
      printf("| %s | %5d | %5d | %5d | %5d |\n",
        r[:datetime],
        r[:count],
        r[:t3] - r[:t1],
        r[:t2] - r[:t1],
        r[:t3] - r[:t2])
    else
      printf("| %s | %5d | %5d | %s | %s |\n",
        r[:datetime],
        r[:count],
        r[:t3] - r[:t1],' '*5,' '*5)
    end
  }
  puts "+----------------------------+-------+-------+-------+-------+"
end

ARGV.each{|arg|
  analyze(arg)
}
