require File.dirname(__FILE__) + '/../spec_helper'

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
