package ProblemaDosBarbeiros;

import java.util.LinkedList;
import java.util.Queue;

public class Barbearia {
    private int qtdBabeiro;
    private int qtdCadeiras;
    private Queue<Cliente> filaClientes;
    private int clientesEsperando;
    private boolean fechado = false;

    public Barbearia(int qtdBabeiro, int qtdCadeiras) {
        this.qtdBabeiro = qtdBabeiro;
        this.qtdCadeiras = qtdCadeiras;
        this.filaClientes = new LinkedList<>();
        this.clientesEsperando = 0;
    }

    // Método para que o cliente tente cortar o cabelo
    public synchronized boolean cortaCabelo(Cliente cliente) {
        if (fechado) return false;  // Barbearia está fechada

        if (clientesEsperando == qtdCadeiras) {
            return false;  // Barbearia lotada
        } else {
            filaClientes.offer(cliente);
            clientesEsperando++;
            System.out.println("Cliente " + cliente.id + " esperando corte...");
            notifyAll();  // Acorda os barbeiros
            while (filaClientes.contains(cliente) && !fechado) {
                try {
                    wait();  // Cliente espera até ser atendido ou até a barbearia fechar
                } catch (InterruptedException e) {
                    e.printStackTrace();
                }
            }
            return !fechado;  // Retorna falso se a barbearia fechou antes do corte
        }
    }

    // Método para os barbeiros pegarem o próximo cliente
    public synchronized Cliente proximoCliente() {
        while (filaClientes.isEmpty() && !fechado) {
            try {
                wait();  // Barbeiro espera por clientes ou pela barbearia fechar
            } catch (InterruptedException e) {
                e.printStackTrace();
            }
        }
        return filaClientes.poll();  // Retorna o próximo cliente, ou null se a barbearia fechou
    }

    // Método para indicar que o corte foi terminado
    public synchronized void corteTerminado(Cliente cliente) {
        System.out.println("Cliente " + cliente.id + " terminou o corte... saindo da barbearia!");
        clientesEsperando--;
        notifyAll();  // Acorda clientes que estão esperando para entrar
    }

    // Método para fechar a barbearia
    public synchronized void fecharBarbearia() {
        fechado = true;
        System.out.println("Barbearia fechando... Acordando todos os clientes e barbeiros.");
        notifyAll();  // Acorda todas as threads esperando (clientes e barbeiros)
    }

    public boolean isFechado() {
        return fechado;
    }
}
