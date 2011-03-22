require File.dirname(__FILE__) + '/spec_helper'

describe Mustang::Runtime do
  describe "#initialize" do
    context "when initial variables given" do
      it "sets them within global context" do
        rt = Mustang::Runtime.new(:foo => "bar", :spam => "eggs")
        rt[:spam].should == "eggs"
      end
    end
  end

  describe "#evaluate" do
    it "properly evaluates given js code" do
      subject.evaluate("10+20").should == "30"
    end

    context "when local variables given" do
      it "allows to access it from current script" do
        subject.evaluate("10+foo", :foo => 30).should == "40"
      end
    end
  end

  describe "#[]= and #[]" do
    it "sets and gets given variable in global context" do
      subject[:foo] = "bar"
      subject[:foo].should == "bar"
    end

    it "makes set variables available in #evaluate" do
      subject[:spam] = "eggs"
      subject.evaluate("spam").should == "eggs"
    end

    context "reflections" do
      it "properly reflects nil value to/from v8" do
        subject[:foo] = nil
        subject[:foo].should == nil
      end

      it "properly reflects boolean value to/from v8" do
        subject[:foo] = false
        subject[:foo].should == false
        subject[:bar] = true
        subject[:bar].should == true
      end

      it "converts symbol value as string" do
        subject[:foo] = :bar
        subject[:foo].should == "bar"
      end

      it "properly reflects fixnum value" do
        subject[:foo] = 1
        subject[:foo].should == 1
      end

      it "properly reflects string value" do
        subject[:foo] = 1.2
        subject[:foo].should == 1.2
      end

      it "properly reflects arrays"
      it "converts hash to object"
      it "properly reflects objects"
      it "properly reflects functions"
      it "tires to reflect externals"
    end

    describe "#load" do
      it "loads and evaluates given list of files" do
        files = []
        files << File.expand_path("../fixtures/test1.js", __FILE__)
        files << File.expand_path("../fixtures/test2.js", __FILE__)
        
        subject.load(*files)
        subject.evaluate("foo").should == "test1foo"
        subject.evaluate("bar").should == "test2bar"
      end

      it "raises Mustang::ScriptNotFoundError when given file doesn't exist" do
        expect { subject.load("filenotexists") }.to raise_error(Mustang::ScriptNotFoundError)
      end
    end
  end

  describe "global object" do
    context "V8 object" do
      it "is defined" do
        subject.evaluate("V8").should == "[object Object]"
      end
      
      it "#version method returns engine version" do
        subject.evaluate("V8.version").should =~ /\d.\d.\d/
      end
    end

    context "Mustang object" do
      it "is defined" do
        subject.evaluate("Mustang").should == "[object Object]"
      end
    end
  end
end
