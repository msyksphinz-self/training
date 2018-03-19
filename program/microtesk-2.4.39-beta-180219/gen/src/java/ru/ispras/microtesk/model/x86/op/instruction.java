/*
 * Copyright (c) Institute for System Programming of the Russian Academy of Sciences
 * All Rights Reserved
 *
 * Institute for System Programming of the Russian Academy of Sciences (ISP RAS)
 * 25 Alexander Solzhenitsyn st., Moscow, 109004, Russia
 * http://www.ispras.ru
 *
 * The present file was automatically generated on the basis of formal specifications.
 * It is distributed under the same terms and conditions as the derived specifications.
 *
 * N.B. PLEASE DO NOT MODIFY THIS FILE.
 */

package ru.ispras.microtesk.model.x86.op;

import java.util.Map;
import java.math.BigInteger;
import ru.ispras.microtesk.model.Execution;
import ru.ispras.microtesk.model.ProcessingElement;
import ru.ispras.microtesk.model.data.*;
import ru.ispras.microtesk.model.memory.*;
import ru.ispras.microtesk.model.*;
import ru.ispras.microtesk.model.x86.PE;
import ru.ispras.microtesk.model.x86.TempVars;

import static ru.ispras.microtesk.model.x86.TypeDefs.*;

public final class instruction extends IsaPrimitive {
  private static final class Info extends IsaPrimitiveInfoAnd {
    Info() {
      super(
          IsaPrimitiveKind.OP,
          "instruction",
          instruction.class,
          null
          );
      addArgument("operation", x86.INFO);}

    @Override
    public IsaPrimitive create(final Map<String, IsaPrimitive> args) {
      final IsaPrimitive operation = (IsaPrimitive) getArgument("operation", args);

      return new instruction(operation);
    }
  }
  public static final IsaPrimitiveInfoAnd INFO = new Info();

  public final IsaPrimitive operation;

  public instruction(final IsaPrimitive operation) {
    assert x86.INFO.isSupported(operation);

    this.operation = operation;

    addArgument("operation", operation);
  }

  @Override
  public String syntax(final TemporaryVariables tempVars) {
    final TempVars vars__ = (TempVars) tempVars;
    init(vars__);
    return operation.text(vars__);
  }

  @Override
  public String image(final TemporaryVariables tempVars) {
    final TempVars vars__ = (TempVars) tempVars;
    init(vars__);
    return operation.image(vars__);
  }

  @Override
  public void init(final TemporaryVariables tempVars) {
    final TempVars vars__ = (TempVars) tempVars;
    vars__.instruction_image_size.access().store(Data.valueOf(SIZE, 0x0));
  }

  @Override
  public void action(final ProcessingElement procElem, final TemporaryVariables tempVars) {
    final PE pe__ = (PE) procElem;
    final TempVars vars__ = (TempVars) tempVars;
    init(vars__);
    pe__.BRANCH.access().store(Data.valueOf(BIT, 0x0));
    pe__.IP.access().store(pe__.program_counter.access().bitField(15, 0).load());
    operation.execute(pe__, vars__);
    vars__.jump_address.access().store(pe__.NEXTPC.access().load());
    pe__.NEXTPC.access().store(Data.valueOf(WORD, 0x0));
    if (pe__.BRANCH.access().load().equals(Data.valueOf(BIT, 0x1))) {
      pe__.program_counter.access().store(Data.zeroExtend(addR, pe__.CS.access().load()).shiftLeft(Data.valueOf(addR, 0x4)).add(Data.zeroExtend(addR, vars__.jump_address.access().load())));
      pe__.IP.access().store(vars__.jump_address.access().load());
    } else {
      pe__.IP.access().store(pe__.IP.access().load().add(Data.zeroExtend(WORD, vars__.instruction_image_size.access().load())));
      pe__.program_counter.access().store(Data.zeroExtend(addR, pe__.CS.access().load()).shiftLeft(Data.valueOf(addR, 0x4)).add(Data.zeroExtend(addR, pe__.IP.access().load())));
    }
  }
}