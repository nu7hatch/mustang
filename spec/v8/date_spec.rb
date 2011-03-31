require File.dirname(__FILE__) + '/../spec_helper'

describe V8::Date do 
  subject { V8::Date }
  setup_context
  
  before do
    @now = Time.now
  end

  it "inherits V8::Value" do
    subject.new(@now).should be_kind_of(V8::Value)
  end

  describe ".new" do
    it "creates new v8 date based on passed value" do
      subject.new(@now).to_i.should be_eql(@now.to_i)
    end
  end

  describe "#to_i" do
    it "returns fixnum value of represented date" do
      subject.new(@now).to_i.should == @now.to_i
    end
  end

  describe "#to_time" do
    it "returns Time representation" do
      subject.new(@now).to_time.to_i.should == @now.to_i
    end
  end

  describe "an instance" do
    it "is comparable" do
      date = subject.new(@now)
      date.should > @now-10
      date.should < @now+10
    end
    
    it "is delegated properly" do
      date = subject.new(@now)
      date.delegate.should == date.to_time
    end
  end
end
