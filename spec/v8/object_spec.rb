require File.dirname(__FILE__) + '/../spec_helper'

describe V8::Object do
  subject { V8::Object }
  setup_context

  it "inherits V8::Value" do
    subject.new(:foo => 1).should be_kind_of(V8::Value)
  end
  
  describe ".new" do
    it "creates new v8 object" do
      subject.new.should be_kind_of(V8::Object)
    end

    context "when hash with properties passed" do
      it "creates object with set given properties" do
        obj = subject.new(:foo => 1, :bar => 2)
        obj[:foo].should == 1
        obj[:bar].should == 2
      end
    end
  end
  
  describe "#[] and #[]=" do
    it "sets and gets values of given key from current object" do
      obj = subject.new
      obj[:foo] = "bar"
      obj[:foo].should == "bar"
    end

    it "can get properties by numeric keys" do
      obj = subject.new
      obj[1] = 'foo'
      obj[1].should == 'foo'
    end

    it "are aliased with #get and #set" do
      obj = subject.new
      obj.set(:foo, "bar")
      obj.get(:foo).should == "bar"
    end
  end

  describe "#keys" do
    let(:obj) { subject.new(:foo => 1, :bar => 2) }

    it "returns list of keys belonging to an object" do
      obj.keys.should == ['foo', 'bar']
    end

    it "is aliased with #properties" do
      obj.properties.should == ['foo', 'bar']
    end
  end

  describe "an instance" do
    it "is enumerable" do
      res = {}
      obj = subject.new(:foo => 1, :bar => 2)
      obj.each {|k,v| res[k.to_s] = v }
      res['foo'].should == 1
      res['bar'].should == 2
    end

    it "is delegated properly" do
      obj = subject.new(:foo => 1, :bar => 2)
      obj.delegate['foo'].should == 1
      obj.delegate['bar'].should == 2
    end
  end
end
