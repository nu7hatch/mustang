# -*- coding: utf-8 -*-
require File.dirname(__FILE__) + '/spec_helper'

describe V8 do
  it "responds to .version" do
    subject.should respond_to(:version)
    subject.version.should =~ /^\d.\d.\d$/
  end

  it "responds to .dead?" do
    subject.should respond_to(:dead?)
    subject.should_not be_dead
  end

  it "responds to .alive?" do
    subject.should respond_to(:alive?)
    subject.should be_alive
  end
end

describe V8::Context do
  describe "#evaluate" do
    it "evaluates given javascript source" do
      subject.evaluate("var a = 'foo'; a;", "<eval>").should == 'foo'
    end

    it "returns result converted to ruby object" do
      result = subject.evaluate("'foo'", "<eval>")
      result.should be_kind_of(V8::String)
      result.to_s.should == 'foo'
    end

    it "is aliased by #eval" do
      subject.eval("'foo'", "<eval>").should == 'foo'
    end
  end

  describe "#[]" do
    it "gets value of specified variable from the global prototype" do
      subject.evaluate("var foo='bar'", "<eval>")
      subject['foo'].should == 'bar'
    end

    it "is aliased by #get" do
      subject.evaluate("var foo='bar'", "<eval>")
      subject.get('foo').should == 'bar'
    end
  end

  describe "#[]=" do
    it "stores given variable within the global prototype" do
      subject["foo"] = "bar"
      subject.evaluate("foo", "<eval>").should == "bar"
      subject[:spam] = :eggs
      subject.evaluate("spam", "<eval>").should == "eggs"
    end

    it "is aliased by #set" do
      subject.set(:foo, "bar")
      subject.evaluate("foo", "<eval>").should == "bar"
    end
  end

  describe "#global" do
    it "returns the global object for current context" do
      subject.global.should be_kind_of(V8::Object)
    end
  end

  describe "#enter" do
    subject { V8::Context.new }

    context "when block given" do
      it "enters to context evaluates passed block, and exits after execution" do
        entered = false
        subject.enter { |cxt| entered = cxt.entered? }
        entered.should be_true
        subject.should_not be_entered
      end
    end

    context "when no block given" do
      it "enters to context" do
        subject.enter.should be_true
        subject.should be_entered
      end
    end
  end

  describe "#exit" do
    it "exits from context" do
      subject.exit.should_not be
      subject.should_not be_entered
    end
  end
end

describe "V8 data types" do
  let(:cxt) { V8::Context.new }
  before { cxt.enter }
  after { cxt.exit }

  describe V8::Object do
    subject { V8::Object }

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
    end
  end

  describe V8::String do
    subject { V8::String }

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

  describe V8::Integer do 
    subject { V8::Integer }

    it "inherits V8::Object" do
      subject.new(10).should be_kind_of(V8::Object)
    end

    describe ".new" do
      it "creates new v8 integer based on passed value" do
        subject.new(10).to_i.should == 10
        subject.new(-10).to_i.should == -10
      end
    end

    describe "#to_i" do
      it "returns fixnum value of represented integer" do
        subject.new(10).to_i.should == 10
      end
    end

    describe "#to_s" do
      it "returns string representation of referenced value" do
        subject.new(10).to_s.should == "10"
      end
    end

    describe "#to_f" do
      it "returns float representation of referenced value" do
        subject.new(10).to_f.should == 10.0
      end
    end

    describe "an instance" do
      it "is comparable" do
        int = subject.new(10)
        int.should == 10
        int.should_not == 20
        int.should > 5
        int.should < 15
        int.should <= 10
        int.should >= 10
      end
    end
  end

  describe V8::Array do 
    subject { V8::Array }

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

  describe V8::Function do
    subject { V8::Function }

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

  describe "Typecasting" do
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
end
