require File.dirname(__FILE__) + '/../spec_helper'

describe V8::Regexp do 
  subject { V8::Regexp }
  setup_context
  
  it "inherits V8::Number" do
    subject.new(/foo(bar)?/).should be_kind_of(V8::Value)
  end

  describe ".new" do
    context "when ruby regexp passed" do
      it "creates new v8 regexp based on it" do
        subject.new(/foo(bar)?/im).should == /foo(bar)?/im
      end
    end

    context "when string and flags passed" do
      it "creates new v8 regexp based on them" do
        subject.new("foo(bar)?", Regexp::IGNORECASE).should == /foo(bar)?/i
      end
    end

    context "when string only passed" do
      it "creates new v8 regexp based on it" do
        subject.new("foo(bar)?").should == /foo(bar)?/
      end
    end
  end

  describe "#source" do
    it "returns regexp source" do
      subject.new("foo(bar.*)").source.should == "foo(bar.*)"
    end
  end

  describe "#options" do
    it "returns ruby regexp flags value" do
      subject.new("foo", Regexp::IGNORECASE).options.should == 1
      subject.new("foo", Regexp::EXTENDED).options.should == 2
      subject.new("foo", Regexp::MULTILINE).options.should == 4
    end
  end

  describe "#flags" do
    it "returns v8 regexp flags value" do
      subject.new("foo", Regexp::EXTENDED).flags.should == 1
      subject.new("foo", Regexp::IGNORECASE).flags.should == 2
      subject.new("foo", Regexp::MULTILINE).flags.should == 4
    end
  end

  describe "#to_regexp" do
    it "returns fixnum value of represented regexp" do
      subject.new(/foo(bar)?/).to_regexp.should == /foo(bar)?/
    end
  end

  describe "an instance" do
    it "is delegated properly" do
      pending
      rxp = subject.new(3)
      rxp.delegate.should == int.to_i
    end
  end
end
