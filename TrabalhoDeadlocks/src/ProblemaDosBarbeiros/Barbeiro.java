package ProblemaDosBarbeiros;

import java.util.Random;

public class Barbeiro extends Pessoa implements Runnable {
    private Barbearia barbearia;
    private boolean running = true;

    public Barbeiro(int id, Barbearia barbearia) {
        super(id);
        this.barbearia = barbearia;
    }

    @Override
    public void run() {
        Random random = new Random();
        while (running) {
            try {
                Cliente cliente = barbearia.proximoCliente();  // Espera pelo próximo cliente
                if (cliente != null) {
                    System.out.println("Barbeiro " + id + " cortando cabelo do Cliente " + cliente.id);
                    Thread.sleep((random.nextInt(3) + 1) * 1000);  // Corta o cabelo por 1 a 3 segundos
                    barbearia.corteTerminado(cliente);
                } else if (barbearia.isFechado()) {
                    System.out.println("Barbeiro " + id + " encerrando suas atividades.");
                    break;
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
