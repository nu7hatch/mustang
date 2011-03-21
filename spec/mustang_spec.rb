require File.dirname(__FILE__) + '/spec_helper'

USED_V8_VERSION = "3.1.6"
ENGINE_VERSION = "0.0.2"

describe Mustang do
  describe ".version" do
    it "returns current version" do
      subject.version.should == ENGINE_VERSION
    end
  end
end

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
  end  
end
