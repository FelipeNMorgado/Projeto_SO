package ProblemaDosBarbeiros;

import java.util.Scanner;

public class SleepingBarber {
    public static void main(String[] args) {
        Scanner scanner = new Scanner(System.in);
        System.out.println("Digite a quantidade de barbeiros: ");
        int qtdBabeiro = scanner.nextInt();
        System.out.println("Digite a quantidade de cadeiras: ");
        int qtdCadeiras = scanner.nextInt();
        System.out.println("Digite a quantidade de clientes: ");
        int qtdClientes = scanner.nextInt();
        scanner.close();

        Barbearia barbearia = new Barbearia(qtdBabeiro, qtdCadeiras);

        Thread[] barbeiros = new Thread[qtdBabeiro];
        Barbeiro[] barbeirosObjs = new Barbeiro[qtdBabeiro];
        for (int i = 0; i < qtdBabeiro; i++) {
            Barbeiro barbeiro = new Barbeiro(i, barbearia);
            barbeirosObjs[i] = barbeiro;
            barbeiros[i] = new Thread(barbeiro);
            barbeiros[i].start();
        }

        // Inicializa clientes
        Thread[] clientes = new Thread[qtdClientes];
        Cliente[] clientesObjs = new Cliente[qtdClientes];
        for (int i = 0; i < qtdClientes; i++) {
            Cliente cliente = new Cliente(i, barbearia);
            clientesObjs[i] = cliente;
            clientes[i] = new Thread(cliente);
            clientes[i].start();
        }

        // Simula a barbearia por 30 segundos (ou o tempo que você definir)
        try {
            Thread.sleep(30000);
        } catch (InterruptedException e) {
            e.printStackTrace();
        }

        // Fechar barbearia e parar threads
        barbearia.fecharBarbearia();
        
        for (Cliente cliente : clientesObjs) {
            cliente.parar();
        }
        for (Barbeiro barbeiro : barbeirosObjs) {
            barbeiro.parar();
        }

        // Espera que todas as threads terminem
        for (Thread clienteThread : clientes) {
            try {
                clienteThread.join();
            } catch (InterruptedException e) {
                e.printStackTrace();
            }
        }

        for (Thread barbeiroThread : barbeiros) {
            try {
                barbeiroThread.join();
            } catch (InterruptedException e) {
                e.printStackTrace();
            }
        }

        System.out.println("Simulação da barbearia terminou.");
        
    }
}
