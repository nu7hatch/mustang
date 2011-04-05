This code is proudly sponsored by [Cubox, Agile Rails Devshop](http://cuboxsa.com)

# Mustang - V8 engine in Ruby's body

Mustang is ruby proxy library for awesome Google V8 JavaScript engine. 

## Installation

Using rubygems:

    $ gem install mustang

Manually:

    $ git clone git://github.com/nu7hatch/mustang.git
    $ cd mustang
    $ rake compile
    $ rake install

## Usage

For now it's only proof of concept. It implements very simple evaluation of javascript code:

    require 'mustang'
    
    cxt = Mustang::Context.new
    cxt.eval("'Hello' + ' World!';") # => 'Hello World!'
    cxt.eval("var a=1;")
    cxt.eval("a+5;") # => 6

## Note on Patches/Pull Requests
 
* Fork the project.
* Make your feature addition or bug fix.
* Add tests for it. This is important so I don't break it in a
  future version unintentionally.
* Commit, do not mess with rakefile, version, or history.
  (if you want to have your own version, that is fine but bump version in a commit by itself I can ignore when I pull)
* Send me a pull request. Bonus points for topic branches.

## Copyright

Copyright (c) 2010 Kriss 'nu7hatch' Kowalik. See LICENSE for details.
