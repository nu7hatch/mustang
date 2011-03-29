require File.dirname(__FILE__) + '/../spec_helper'

describe V8::Function do
  subject { V8::Function }
  setup_context

  it "inherits V8::Object" do
    pending
    subject.new(lambda{ "foo" }).should be_kind_of(V8::Object)
  end

  describe ".new" do
    it "creates new function pointed to proc/lambda"
  end

  describe "#call" do
    it "exectutes referenced javascript function" do
      func = cxt.eval("var f = function(foo, bar) { return foo+bar }; f;", "<eval>")
      func.call(10, 20).should == 30
    end

    context "when different `this` object is assigned" do
      it "executes function on it" do
        func = cxt.eval("var f = function(foo) { return this+foo }; f;", "<eval>")
        func.this = V8::Integer.new(10)
        func.call(10).should == 20;
      end
    end
  end
end
