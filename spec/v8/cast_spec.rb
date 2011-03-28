require File.dirname(__FILE__) + '/../spec_helper'

describe "Typecasting" do
  setup_context

  it "converts js null to nil" do
    cxt.eval("null", "<eval>").should_not be
  end

  it "converts js undefined value to nil" do
    cxt.eval("undefined", "<eval>").should_not be
  end

  it "converts js boolean properly" do
    cxt.eval("true", "<eval>").should be_true
    cxt.eval("false", "<eval>").should be_false
  end

  it "converts js string properly" do
    cxt.eval("'foo'", "<eval>").should == "foo"
  end

  it "converts js integers properly" do
    cxt.eval("1", "<eval>").should == 1
    cxt.eval("-1", "<eval>").should == -1
  end

  it "converts js arrays properly" do
    cxt.eval("[1,2,3]", "<eval>").should == [1,2,3]
  end

  it "converts js objects properly" do
    obj = cxt.eval("var o = {foo: 1, bar: 2, func: function() { return 1 }}; o", "<eval>")
    obj[:foo].should == 1
    obj[:bar].should == 2
    obj[:func].should be_kind_of(V8::Function)
  end
end
