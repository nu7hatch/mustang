require File.dirname(__FILE__) + '/../../spec_helper'

describe Mustang::V8::Array do 
  subject { Mustang::V8::Array }
  setup_context

  it "inherits Mustang::V8::Object" do
    subject.new(1,2,3).should be_kind_of(Mustang::V8::Object)
  end  

  it "is kind of Array" do
    subject.new(1,2,3).should be_kind_of Array
  end
  
  describe ".new" do
    context "when no context entered" do
      it "should raise error" do
        Mustang::V8::Context.exit_all!
        expect { subject.new(1,2,3) }.to raise_error(RuntimeError, "can't create V8 object without entering into context")
      end
    end

    context "when no params given" do
      it "creates empty array" do
        ary = subject.new
        ary.should be
        ary.should == []
      end
    end
    
    context "when array given" do
      it "creates v8 array based on it" do
        ary = subject.new(1,2,3)
        ary.should == [1,2,3]
      end
    end
  end

  describe "#length" do
    it "returns size of array" do
      subject.new(1,2).length.should == 2
    end
    
    it "is aliased with #size" do
      subject.new(1,2).size.should == 2
    end
  end

  describe "#to_a" do
    it "returns ruby array representation of referenced v8 array" do
      subject.new(1,2).to_a.should == [1,2]
    end
  end

  describe "#push" do
    it "appends given object to array" do
      ary = subject.new
      ary.push("foo")
      ary[0].should == "foo"
    end

    it "is aliased with #<<" do
      ary = subject.new
      ary.push("foo")
      ary[0].should == "foo"
    end
  end

  describe "an instance" do
    it "is comparable" do
      ary = subject.new(1,2,3)
      ary.should == [1,2,3]
      ary.should_not == [2,3,4]
      ary.should > [1,2]
      ary.should < [1,2,3,4]
      ary.should <= [1,2,3,4]
      ary.should >= [1,2,3]
    end

    it "is enumerable" do
      res = []
      ary = subject.new(1,2,3)
      ary.each {|x| res << x }
      res.should == [1,2,3]
    end

    it "is delegated properly" do
      ary = subject.new(1,2,3)
      ary.delegate.should == ary.to_a
    end
  end

  describe "in context" do
    it "object reflected twice should be explicitly the same" do
      cxt.eval("foo = [1,2,3]", "<eval>")
      cxt[:foo].object_id.should == cxt[:foo].object_id
    end
  end
end
