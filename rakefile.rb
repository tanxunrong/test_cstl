
task :default => [:install]

desc "install"
task :install =>["test_filter_lock","test_bakery_lock"] do

end


file "bakery_lock.o" => ["bakery_lock.h","bakery_lock.c"] do
	sh %Q{clang -c -Wall -std=c99 bakery_lock.c} do |ok,res|
		if !ok
			puts "build fail #{res.exitstatus}"
		end
	end
end


file "filter_lock.o" => ["filter_lock.h","filter_lock.c"] do
	sh %Q{clang -c -Wall -std=c99 filter_lock.c} do |ok,res|
		if !ok
			puts "build fail #{res.exitstatus}"
		end
	end
end


file "test_filter_lock" => ["test/test_filter_lock.c","filter_lock.o"] do
	sh %Q{clang -c -Wall -std=c99 test/test_filter_lock.c} do |ok,res|
		if !ok
			puts "build fail #{res.exitstatus}"
		end
	end
	sh %Q{clang filter_lock.o test_filter_lock.o -lpthread -o test_filter_lock}
end


file "test_bakery_lock" => ["test/test_bakery_lock.c","bakery_lock.o"] do
	sh %Q{clang -c -Wall -std=c99 test/test_bakery_lock.c} do |ok,res|
		if !ok
			puts "build fail #{res.exitstatus}"
		end
	end
	sh %Q{clang bakery_lock.o test_bakery_lock.o -lpthread -o test_bakery_lock}
end

desc "clean"
task :clean do
	sh %Q{rm test_*}
	sh %Q{rm *.o}
end