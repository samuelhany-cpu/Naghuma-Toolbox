import { Wand2, Droplet, Sun, Contrast, Sparkles, Wind } from 'lucide-react';

interface ProcessesTabProps {
  onProcessChange: (processes: ImageProcesses) => void;
  processes: ImageProcesses;
}

export interface ImageProcesses {
  brightness: number;
  contrast: number;
  saturation: number;
  blur: number;
  hue: number;
  sharpness: number;
}

export function ProcessesTab({ onProcessChange, processes }: ProcessesTabProps) {
  const handleChange = (key: keyof ImageProcesses, value: number) => {
    onProcessChange({ ...processes, [key]: value });
  };

  const processControls = [
    { key: 'brightness' as const, label: 'Brightness', icon: Sun, min: -100, max: 100, default: 0 },
    { key: 'contrast' as const, label: 'Contrast', icon: Contrast, min: -100, max: 100, default: 0 },
    { key: 'saturation' as const, label: 'Saturation', icon: Droplet, min: -100, max: 100, default: 0 },
    { key: 'hue' as const, label: 'Hue Rotate', icon: Wand2, min: 0, max: 360, default: 0 },
    { key: 'blur' as const, label: 'Blur', icon: Wind, min: 0, max: 10, default: 0 },
    { key: 'sharpness' as const, label: 'Sharpness', icon: Sparkles, min: 0, max: 100, default: 0 },
  ];

  return (
    <div className="space-y-4 p-4">
      <h3 className="text-fuchsia-300">Image Adjustments</h3>
      
      {processControls.map(({ key, label, icon: Icon, min, max, default: defaultValue }) => (
        <div key={key} className="space-y-2">
          <div className="flex items-center justify-between">
            <label className="flex items-center gap-2 text-purple-200">
              <Icon className="w-4 h-4" />
              {label}
            </label>
            <span className="text-xs text-purple-300 bg-purple-900/40 px-2 py-1 rounded">
              {processes[key]}
            </span>
          </div>
          <input
            type="range"
            min={min}
            max={max}
            value={processes[key]}
            onChange={(e) => handleChange(key, Number(e.target.value))}
            className="w-full h-2 bg-purple-900/40 rounded-lg appearance-none cursor-pointer slider"
            style={{
              background: `linear-gradient(to right, #e879f9 0%, #e879f9 ${((processes[key] - min) / (max - min)) * 100}%, #3d3250 ${((processes[key] - min) / (max - min)) * 100}%, #3d3250 100%)`
            }}
          />
        </div>
      ))}

      <style jsx>{`
        .slider::-webkit-slider-thumb {
          appearance: none;
          width: 16px;
          height: 16px;
          background: linear-gradient(135deg, #e879f9, #c026d3);
          border-radius: 50%;
          cursor: pointer;
          box-shadow: 0 0 8px rgba(232, 121, 249, 0.5);
        }

        .slider::-moz-range-thumb {
          width: 16px;
          height: 16px;
          background: linear-gradient(135deg, #e879f9, #c026d3);
          border-radius: 50%;
          cursor: pointer;
          border: none;
          box-shadow: 0 0 8px rgba(232, 121, 249, 0.5);
        }
      `}</style>
    </div>
  );
}
