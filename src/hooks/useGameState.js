import { useState, useEffect, useCallback } from "react";
import { DAILY_QUESTS, ITEMS, WILD_POKEMON } from "../data/pokemon";

const STORAGE_KEY = "pokerogue_tasks_v1";

const getInitialState = () => {
  try {
    const saved = localStorage.getItem(STORAGE_KEY);
    if (saved) return JSON.parse(saved);
  } catch (e) {}
  return {
    trainerName: "",
    bag: {},           // { itemId: quantity }
    caught: [],        // array of pokemon objects
    quests: [],
    lastQuestDate: null,
    totalCompleted: 0,
    wins: 0,
    losses: 0,
  };
};

export const useGameState = () => {
  const [state, setState] = useState(getInitialState);
  const [rewardItems, setRewardItems] = useState(null); // 3 items to choose from
  const [notification, setNotification] = useState(null);

  useEffect(() => {
    localStorage.setItem(STORAGE_KEY, JSON.stringify(state));
  }, [state]);

  // Generate daily quests
  useEffect(() => {
    const today = new Date().toDateString();
    if (state.lastQuestDate !== today) {
      generateDailyQuests(today);
    }
  }, []);

  const generateDailyQuests = useCallback((dateStr) => {
    const today = dateStr || new Date().toDateString();
    const shuffled = [...DAILY_QUESTS].sort(() => Math.random() - 0.5);
    const quests = shuffled.slice(0, 4).map((text, i) => ({
      id: `${today}-${i}`,
      text,
      completed: false,
      date: today,
    }));
    setState((prev) => ({ ...prev, quests, lastQuestDate: today }));
  }, []);

  const completeQuest = useCallback((questId) => {
    setState((prev) => {
      const quest = prev.quests.find((q) => q.id === questId);
      if (!quest || quest.completed) return prev;
      return {
        ...prev,
        quests: prev.quests.map((q) => q.id === questId ? { ...q, completed: true } : q),
        totalCompleted: prev.totalCompleted + 1,
      };
    });
    // Propose 3 random items as reward
    const pool = [...ITEMS].sort(() => Math.random() - 0.5).slice(0, 3);
    setRewardItems(pool);
  }, []);

  const pickRewardItem = useCallback((itemId) => {
    setState((prev) => ({
      ...prev,
      bag: { ...prev.bag, [itemId]: (prev.bag[itemId] || 0) + 1 },
    }));
    setRewardItems(null);
    notify("Objet ajouté au sac !");
  }, []);

  const useItemInBattle = useCallback((itemId) => {
    setState((prev) => {
      const qty = prev.bag[itemId] || 0;
      if (qty <= 0) return prev;
      const newBag = { ...prev.bag, [itemId]: qty - 1 };
      if (newBag[itemId] === 0) delete newBag[itemId];
      return { ...prev, bag: newBag };
    });
  }, []);

  const catchPokemon = useCallback((pokemon, ballId) => {
    setState((prev) => {
      // Consume ball
      const qty = prev.bag[ballId] || 0;
      if (qty <= 0) return prev;
      const newBag = { ...prev.bag, [ballId]: qty - 1 };
      if (newBag[ballId] === 0) delete newBag[ballId];
      // Add to caught if not duplicate
      const alreadyCaught = prev.caught.find((p) => p.id === pokemon.id);
      const newCaught = alreadyCaught
        ? prev.caught
        : [...prev.caught, { ...pokemon, caughtAt: new Date().toLocaleDateString() }];
      return { ...prev, bag: newBag, caught: newCaught };
    });
  }, []);

  const recordBattleResult = useCallback((won) => {
    setState((prev) => ({
      ...prev,
      wins: won ? prev.wins + 1 : prev.wins,
      losses: won ? prev.losses : prev.losses + 1,
    }));
  }, []);

  const setTrainerName = useCallback((name) => {
    setState((prev) => ({ ...prev, trainerName: name }));
  }, []);

  const notify = (msg) => {
    setNotification(msg);
    setTimeout(() => setNotification(null), 2500);
  };

  const resetGame = useCallback(() => {
    localStorage.removeItem(STORAGE_KEY);
    setState(getInitialState());
  }, []);

  return {
    state,
    rewardItems,
    notification,
    completeQuest,
    pickRewardItem,
    useItemInBattle,
    catchPokemon,
    recordBattleResult,
    setTrainerName,
    generateDailyQuests,
    resetGame,
  };
};
