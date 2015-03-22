class WidnerVST
  def initialize
    @width   = 0
    @mid     = 1
    @side    = 1
  end

  def set_parameter(index, value)
    @width = value if index == 0
  end

  def parameter_name(index)
    index == 0 ? "Width" : "empty"
  end

  def parameter_value(index)
    @pan if index == 0
  end

  def parameter_display_value(index)
    (@width * 10).to_i.to_s if index == 0
  end

  def parameter_label(index)
  end

  def process(data)
    length = data[0].length
    length.times do |t|
      data[0][t], data[1][t] = process_lr(data[0][t], data[1][t])
    end
    data
  end

  def process_lr(l,r)
    mid  = 0.5 * (l + r)
    side = 0.5 * (r - l) * @width
    [mid - side, mid + side]
  end
end

WidnerVST.new
