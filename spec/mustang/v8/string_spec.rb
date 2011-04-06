# -*- coding: utf-8 -*-
require File.dirname(__FILE__) + '/../../spec_helper'

describe Mustang::V8::String do
  subject { Mustang::V8::String }
  setup_context

  it "inherits Mustang::V8::Primitive" do
    subject.new("foo").should be_kind_of(Mustang::V8::Primitive)
  end

  describe ".new" do
    context "when no context entered" do
      it "should raise error" do
        Mustang::V8::Context.exit_all!
        expect { subject.new("foo") }.to raise_error(RuntimeError, "can't create V8 object without entering into context")
      end
    end

    it "creates new v8 string based on passed value" do
      subject.new("foo").to_s.should == "foo"
    end
  end

  describe "#to_ascii" do
    it "returns ASCII value of represented string" do
      subject.new("foobar").to_ascii.should == "foobar"
    end
  end

  describe "#to_utf8" do
    it "returns UTF-8 value of represented string" do
      subject.new("zażółć").to_utf8.should == "zażółć"
    end
    
    it "is aliased by #to_s" do
      subject.new("foobar").to_s.should == "foobar"
    end
  end

  describe "an instance" do
    it "is comparable" do
      str = subject.new("foo")
      str.should == "foo"
      str.should_not == "bar"
      str.should > "a"
      str.should < "foobar"
    end
    
    it "is delegated properly" do
      str = subject.new("foo")
      str.delegate.should == str.to_s
    end
  end
end
