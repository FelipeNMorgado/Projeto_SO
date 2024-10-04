package ProblemaDosBarbeiros;

import java.util.Random;

public class Cliente extends Pessoa implements Runnable {
    private Barbearia barbearia;
    private boolean running = true;

    public Cliente(int id, Barbearia barbearia) {
        super(id);
        this.barbearia = barbearia;
    }

    @Override
    public void run() {
        Random random = new Random();
        while (running) {
            try {
                Thread.sleep((random.nextInt(3) + 3) * 1000);  // Espera entre 3 e 5 segundos
                if (!barbearia.cortaCabelo(this) && !barbearia.isFechado()) {
                    System.out.println("Cliente " + id + " tentou entrar na barbearia, mas está lotada... indo dar uma voltinha");
                }
            } catch (InterruptedException e) {
                e.printStackTrace();
            }
        }
    }

    public void parar() {
        this.running = false;
    }
}
