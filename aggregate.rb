
File.open("README.md", 'w') do |file|
    Dir.glob("*").select { |f| File.directory? f}.each do |dir|
        file.puts "# #{dir}"
        file.write File.read(File.join(dir, "README.md"))
        file.puts
        file.puts
    end
end
