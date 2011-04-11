Mustang development is proudly sponsored by [Cubox, Agile Rails Devshop](http://cuboxsa.com)

# Mustang - V8 engine in Ruby's body

Mustang is ruby proxy library for awesome Google V8 JavaScript engine. 

## Motivation

JavaScript integration testing suite which contains more than 4000 steps and running time
more that 2 hours... I think that's enough. There is lot of different ways to handle headless
testing, but they all have lot of issues as well. They are super slow (Selenium, Watir), or
they are not working with ruby 1.9 (Johsnon), or they requires tricky external stuff (Culerity/Celerity,
Zombie). Our goal is to create fast javascript engine working fine on all popular ruby implementations
(1.8.x, 1.9.x, ree, rubinius), and make it core element of headless, virtual browser for testing
purposes (of course integrated with capybara and cucubmer).

## TheRubyRacer and PyV8 influence...

Yeap, [TheRubyRacer](http://github.com/cowboyd/therubyracer) from [Charles Lowell](http://github.com/cowboyd)
and [PyV8](http://code.google.com/p/pyv8/) made big influence on our Mustang code, so let say now big thanks
to the authors...

You can of course ask why we wrote library which seems to be very similar to TheRubyRacer... yeah it seems, but there
are a lot of differences. Here is short comparison of TheRubyRacer and Mustang:

Similarities:

* Both are using similar integration with ruby's garbage collector. Actually, Mustang's code which handles
  that references is just improved version of TheRubyRacer's code (check the <tt>ext/v8/v8_ref.cpp</tt>).
* Both are reflecting JS objects to ruby and vice-versa. 

Diffs:

* Mustang reflects all JavaScript objects and values properly. Properly, means
  all values from within V8 are reflected to <tt>Mustang::V8::*</tt> objects,
  regarding V8 inheritance tree which you can find [here](http://izs.me/v8-docs/classv8_1_1Data.html).
* Once reflected ruby object (and vice-versa) is always the same object in ruby
  world.
* All reflected V8 objects acts exacltly the same as ruby natives. For example
  you can deal with Mustang::V8::Array exactly the same as with native Ruby arrays
  (eg. they are comparable with ruby objects, etc).
* Functions are reflected more accurately. Ruby objects' methods are represented
  properly.
* Classes are converted to V8 function templates, so you can deal with them as with
  prototypes.
* Different approach to deal with contexts, compiling JS scripts and exceptions handling. 
* Support for regexp reflections.
* Way more friendly ruby api.
* A very thorough test suite (over 250 tests passing on all ruby versions)
* Clean codebase, less magic, fully documented.
* Lower memory usage. 

And the most important, like i said before, Mustang is used as core element of our
bigger idea, so it have to exactly fit to our needs. 

## Installation

Before you install mustang gem make sure you have `scons` installed.

    $ sudo apt-get install scons # on debian
    $ sudo pacman -S scons       # on archlinux
    $ brew install scons         # on mac...

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

    cxt[:puts] = method(:puts)
    cxt.eval("puts(a)") # displays "1" on the screen
    cxt[:a] # => 1

### Advanced

TODO: coming soon...

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
