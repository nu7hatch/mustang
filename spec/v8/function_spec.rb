require File.dirname(__FILE__) + '/../spec_helper'

describe V8::Function do
  subject { V8::Function }
  setup_context

  it "inherits V8::Object" do
    subject.new(lambda { "foo" }).should be_kind_of(V8::Object)
  end

  describe ".new" do
    it "creates new function pointed to proc/lambda" do
      func = subject.new(lambda {|bar| "foo#{bar}"})
      func.call("foo").should == "foofoo"
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
        func = V8::Function.new(lambda {|arg| return arg })
        func.call().should be_error
        func.call(1).should == 1
        func = V8::Function.new(lambda {|*args| return args.size })
        func.call().should == 0
        func.call(1,2,3).should == 3
      end
    end
  end
end
