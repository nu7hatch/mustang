require File.dirname(__FILE__) + '/../spec_helper'
require 'stringio'

describe V8::External do
  subject { V8::External }
  setup_context

  it "inherits V8::Value" do
    subject.new(StringIO.new).should be_kind_of(V8::Value)
  end
  
  describe ".new" do
    it "creates new v8 external" do
      obj = StringIO.new
      subject.new(obj).should be_kind_of(V8::External)
    end
  end
  
  describe "#value" do
    it "sets and gets values of given key from current object" do
      obj = StringIO.new
      ext = subject.new(obj)
      ext.value.should == obj
    end

    it "is aliased with #to_obj" do
      obj = StringIO.new
      ext = subject.new(obj)
      ext.to_obj.should == obj
    end
  end

  describe "in context" do
    it "object reflected twice should be explicitly the same" do
      cxt.eval("foo = {a:1, b:2}", "<eval>")
      cxt[:foo].object_id.should == cxt[:foo].object_id
    end
  end
end
