require File.dirname(__FILE__) + '/../spec_helper'

describe V8::Array do 
  subject { V8::Array }
  setup_context
  
  describe ".new" do
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
  end
end
