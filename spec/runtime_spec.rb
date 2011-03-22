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
  end  
end
