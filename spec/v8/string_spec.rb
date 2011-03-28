# -*- coding: utf-8 -*-
require File.dirname(__FILE__) + '/../spec_helper'

describe V8::String do
  subject { V8::String }
  setup_context
  
  it "inherits V8::Object" do
    subject.new("foo").should be_kind_of(V8::Object)
  end

  describe ".new" do
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
      int = subject.new("foo")
      int.should == "foo"
      int.should_not == "bar"
      int.should > "a"
      int.should < "foobar"
    end
  end
end
