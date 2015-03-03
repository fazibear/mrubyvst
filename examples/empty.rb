class VST
  attr_reader :vendor, :product, :version

  def initialize
    @vendor  = 'MRuby powered plugin'
    @product = 'Empty Plugin'
    @name    = 'Empty Vst'
    @version = 0
  end

  def parameter_set(index, value)
    #
  end

  def parameter_get(index)
    1.0
  end

  def parameter_name(index)
    'empty'
  end

  def parameter_display(index)
    ''
  end

  def parameter_label(index)
    ''
  end

  def process_replacing(data)
    data
  end
end
