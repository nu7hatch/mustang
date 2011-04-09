require File.dirname(__FILE__) + '/../../spec_helper'

describe Mustang::V8::Function do
  subject { Mustang::V8::Function }
  setup_context

  it "inherits Mustang::V8::Object" do
    subject.new(lambda { "foo" }).should be_kind_of(Mustang::V8::Object)
  end

  it "is kind of Proc" do
    subject.new(proc {}).should be_kind_of Proc
  end

  describe ".new" do
    context "when no context entered" do
      it "should raise error" do
        Mustang::V8::Context.exit_all!
        expect { subject.new(lambda {}) }.to raise_error(RuntimeError, "can't create V8 object without entering into context")
      end
    end

    context "when given proc/lambda" do
      it "creates new function pointed to it" do
        func = subject.new(lambda {|bar| "foo#{bar}"})
        func.call("foo").should == "foofoo"
      end
    end

    context "when block given" do
      it "creates new function pointed to it" do
        func = subject.new {|bar| "foo#{bar}" }
        func.call("foo").should == "foofoo"
      end
    end

    context "when object method given" do
      it "creates new function pointed to it" do
        class Foo
          def initialize(foo); @foo = foo; end
          def bar(arg); return "foo#{arg}#{@foo}"; end
        end

        foo = Foo.new('bar')
        func = subject.new(foo.method(:bar))
        func.call("foo").should =="foofoobar"
      end
    end
  end

  describe "#call_on" do
    context "when recv is nil" do
      it "executes function on global object" do
        cxt.global[:foo] = 'bar' 
        func = cxt.eval("var f = function(foo) { return this.foo+foo }; f;", "<eval>")
        func.call_on(nil, 'bar').should == 'barbar'
      end
    end

    context "when recv given" do
      it "executes function on it" do
        func = cxt.eval("var f = function(foo) { return this+foo }; f;", "<eval>")
        func.call_on(10.to_v8, 10).should == 20
      end
    end
  end

  describe "#call" do
    it "exectutes referenced javascript function" do
      func = cxt.eval("var f = function(foo, bar) { return foo+bar }; f;", "<eval>")
      func.call(10, 20).should == 30
    end

    context "when bound to dirreferent receiver object" do
      it "executes function on it" do
        func = cxt.eval("var f = function(foo) { return this+foo }; f;", "<eval>")
        func.bind(10.to_v8)
        func.call(10).should == 20
      end
    end

    context "when not bound to any receiver" do
      it "executes function on global object" do
        func = cxt.eval("var f = function() { return this; }; f;", "<eval>")
        func.call().should == cxt.global
      end
    end

    context "when function cause errors" do
      it "returns an caught error object" do
        func = cxt.eval("var f = function() { return 'foo'+notexists; }; f;", "<eval>")
        func.call().should be_error
      end
    end

    context "when wrong numbers of arguments given" do
      it "returns proper error" do
        func = Mustang::V8::Function.new(lambda {|arg| return arg })
        func.call().should be_error
        func.call(1).should == 1
        func = Mustang::V8::Function.new(lambda {|*args| return args.size })
        func.call().should == 0
        func.call(1,2,3).should == 3
      end
    end
  end

  describe "#bind" do
    it "binds function with given object" do
      func = Mustang::V8::Function.new(lambda{})
      func.bind('foo')
      func.receiver.should == 'foo'
    end
  end

  describe "#receiver" do
    context "when no object bound to function" do
      it "returns nil" do
        func = Mustang::V8::Function.new(lambda{})
        func.receiver.should_not be
      end
    end

    context "when object bound to function" do
      it "returns it" do
        func = Mustang::V8::Function.new(lambda{})
        func.bind(1)
        func.receiver.should == 1
      end
    end
  end

  describe "#name= and #name" do
    it "sets and gets function's unique name" do
      func = Mustang::V8::Function.new(lambda{})
      func.name = 'foo'
      func.name.should == 'foo'
    end
  end

  describe "#origin" do
    context "when function has been created from ruby proc/lambda/method" do
      it "returns it" do
        proc = Proc.new {}
        func = Mustang::V8::Function.new(proc)
        func.origin.should == proc
      end
    end

    context "when function has been taken from javascript" do
      it "returns nil" do
        func = cxt.eval("var f = function(){}; f", "<eval>")
        func.origin.should_not be
      end
    end
  end

  describe "#to_proc" do
    context "when function has been created from ruby proc/lambda/method" do
      it "returns it" do
        proc = Proc.new {}
        func = Mustang::V8::Function.new(proc)
        func.to_proc.should == proc
      end
    end

    context "when function has been taken from javascript" do
      it "returns nil" do
        func = cxt.eval("var f = function(){}; f", "<eval>")
        func.to_proc.should_not be
      end
    end
  end
  
  describe "in context" do
    it "object reflected twice should be explicitly the same" do
      cxt.eval("foo = function() {}", "<eval>")
      cxt[:foo].object_id.should == cxt[:foo].object_id
    end
  end
end
