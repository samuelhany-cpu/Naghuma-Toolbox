interface TabButtonProps {
  active: boolean;
  onClick: () => void;
  icon: React.ReactNode;
  label: string;
}

export function TabButton({ active, onClick, icon, label }: TabButtonProps) {
  return (
    <button
      onClick={onClick}
      className={`flex flex-col items-center gap-1 px-4 py-3 transition-all ${
        active
          ? 'bg-gradient-to-b from-fuchsia-500/20 to-transparent border-b-2 border-fuchsia-400 text-fuchsia-300'
          : 'text-purple-300/60 hover:text-purple-200 hover:bg-purple-900/20'
      }`}
    >
      {icon}
      <span className="text-xs">{label}</span>
    </button>
  );
}
