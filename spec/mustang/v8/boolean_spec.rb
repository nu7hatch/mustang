require File.dirname(__FILE__) + '/../../spec_helper'

describe Mustang::V8::Boolean do 
  subject { Mustang::V8::Boolean }
  setup_context
  
  it "inherits Mustang::V8::Data" do
    subject.new(10).should be_kind_of(Mustang::V8::Primitive)
  end

  it "is kind of FalseClass or TrueClass" do
    Mustang::V8::False.should be_kind_of FalseClass
    Mustang::V8::True.should be_kind_of TrueClass
  end
end

shared_examples_for "Boolean" do
  describe "#to_bool" do
    it "returns ruby bool value" do
      subject.to_bool.should == compare
    end
  end

  describe "#true?" do
    it "returns proper value" do
      subject.true?.should == (compare == true)
    end
  end

  describe "#false?" do
    it "returns proper value" do
      subject.false?.should == (compare == false)
    end
  end

  describe "#&" do
    it "acts as ruby bool's method" do
      (subject & true).should == (compare & true)
      (subject & false).should == (compare & false)
    end
  end

  describe "#|" do
    it "acts as ruby bool's method" do
      (subject | true).should == (compare | true)
      (subject | false).should == (compare | false)
    end
  end

  describe "#^" do
    it "acts as ruby bool's method" do
      (subject ^ true).should == (compare ^ true)
      (subject ^ false).should == (compare ^ false)
    end
  end

  describe "#==" do
    it "acts as ruby bool's method" do
      subject.should == compare
      subject.should_not == !compare
    end
  end

  describe "#to_s" do
    it "acts as ruby bool's method" do
      subject.to_s.should == compare.to_s
    end
  end
end

describe Mustang::V8::False do
  let(:compare) { false }
  it_should_behave_like "Boolean"

  it "acts as normal bool in conditional statements" do
    pending
    (subject ? 'foo' : 'bar').should == 'bar'
    (!subject ? 'foo' : 'bar').should == 'foo'
  end
end

describe Mustang::V8::True do
  let(:compare) { true }
  it_should_behave_like "Boolean"

  it "acts as normal bool in conditional statements" do
    pending
    (!subject ? 'foo' : 'bar').should == 'bar'
    (subject ? 'foo' : 'bar').should == 'foo'
  end
end
